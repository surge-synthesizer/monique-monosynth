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

#include "monique_ui_LookAndFeel.h"
#include "monique_ui_MainWindow.h"
#include "monique_core_Datastructures.h"

#define CAN_OPAQUE true
//==============================================================================
//==============================================================================
//==============================================================================
namespace LookAndFeelHelpers
{
static Colour createBaseColour(Colour buttonColour, bool hasKeyboardFocus, bool isMouseOverButton,
                               bool isButtonDown) noexcept
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour(buttonColour.withMultipliedSaturation(sat));

    if (isButtonDown)
        return baseColour.contrasting(0.2f);
    if (isMouseOverButton)
        return baseColour.contrasting(0.1f);

    return baseColour;
}

static TextLayout layoutTooltipText(const String &text, Colour colour, Font &font_) noexcept
{
    const int maxToolTipWidth = 530;

    AttributedString s;
    s.setJustification(Justification::centredLeft);
    s.append(text + String("\n\n___________________________________________________________________"
                           "______\n\nNERVES ARE ON THE EDGE?\n-----------------------\nTool tips "
                           "you can disable in the setup."),
             Font(Typeface::createSystemTypefaceFor(BinaryData::SourceCodeProMedium_otf,
                                                    BinaryData::SourceCodeProMedium_otfSize))
                 .withHeight(15.0f),
             // font_,
             colour);

    TextLayout tl;
    tl.createLayoutWithBalancedLineLengths(s, (float)maxToolTipWidth);
    return tl;
}
} // namespace LookAndFeelHelpers

//==============================================================================
//==============================================================================
//==============================================================================
ComponentColours::ComponentColours() noexcept : midi_learn(Colours::red) { init_themes(); }
ComponentColours::~ComponentColours() noexcept {}

//==============================================================================
void ComponentColours::read_from(XmlElement *xml_) noexcept
{
    XmlElement *xml = xml_->getChildByName("COLOURS");
    if (xml)
    {
        for (int i = 0; i != themes.size(); ++i)
        {
            SectionTheme *theme = themes.getUnchecked(i);

            auto pfx = String("c_") + String(i);

            // This allows us to read old configs even though they aren't particularly valid xml.
            // When they are restreamed they will use the new format
            auto readNewOld = [this, xml](const juce::String &s, const juce::String &def) {
                auto res = xml->getStringAttribute(s, "SENTINEL");
                if (res == "SENTINEL")
                {
                    auto qs = s.substring(2);
                    res = xml->getStringAttribute(qs, def);
                }
                return res;
            };

            theme->area_colour =
                Colour::fromString(readNewOld(pfx + "_area_colour", theme->area_colour.toString()));
            theme->area_font_colour = Colour::fromString(
                readNewOld(pfx + "_area_font_colour", theme->area_font_colour.toString()));
            theme->value_slider_track_colour = Colour::fromString(readNewOld(
                pfx + "_value_slider_track_colour", theme->value_slider_track_colour.toString()));
            theme->value_2_slider_track_colour =
                Colour::fromString(readNewOld(pfx + "_value_2_slider_track_colour",
                                              theme->value_2_slider_track_colour.toString()));
            theme->mod_slider_track_colour = Colour::fromString(readNewOld(
                pfx + "_mod_slider_track_colour", theme->mod_slider_track_colour.toString()));
            theme->disabled_track_colour = Colour::fromString(readNewOld(
                pfx + "_disabled_track_colour", theme->disabled_track_colour.toString()));
            theme->slider_bg_colour = Colour::fromString(
                readNewOld(pfx + "_slider_bg_colour", theme->slider_bg_colour.toString()));
            theme->button_on_font_colour = Colour::fromString(readNewOld(
                pfx + "_button_on_font_colour", theme->button_on_font_colour.toString()));
            theme->button_on_colour = Colour::fromString(
                readNewOld(pfx + "_button_on_colour", theme->button_on_colour.toString()));
            theme->button_off_font_colour = Colour::fromString(readNewOld(
                pfx + "_button_off_font_colour", theme->button_off_font_colour.toString()));
            theme->button_off_colour = Colour::fromString(
                readNewOld(pfx + "_button_off_colour", theme->button_off_colour.toString()));

            theme->oszi_1 =
                Colour::fromString(readNewOld(pfx + "_oszi_1", theme->oszi_1.toString()));
            theme->oszi_2 =
                Colour::fromString(readNewOld(pfx + "_oszi_2", theme->oszi_2.toString()));
            theme->oszi_3 =
                Colour::fromString(readNewOld(pfx + "_oszi_3", theme->oszi_3.toString()));
        }

        midi_learn = Colour::fromString(xml->getStringAttribute("ml_col", Colours::red.toString()));
    }
}
void ComponentColours::save_to(XmlElement *xml_) noexcept
{
    XmlElement *xml = xml_->createNewChildElement("COLOURS");
    if (xml)
    {
        for (int i = 0; i != themes.size(); ++i)
        {
            SectionTheme *theme = themes.getUnchecked(i);

            auto pfx = String("c_") + String(i);

            xml->setAttribute(pfx + "_area_colour", theme->area_colour.toString());
            xml->setAttribute(pfx + "_area_font_colour", theme->area_font_colour.toString());
            xml->setAttribute(pfx + "_value_slider_track_colour",
                              theme->value_slider_track_colour.toString());
            xml->setAttribute(pfx + "_value_2_slider_track_colour",
                              theme->value_2_slider_track_colour.toString());
            xml->setAttribute(pfx + "_mod_slider_track_colour",
                              theme->mod_slider_track_colour.toString());
            xml->setAttribute(pfx + "_disabled_track_colour",
                              theme->disabled_track_colour.toString());
            xml->setAttribute(pfx + "_slider_bg_colour", theme->slider_bg_colour.toString());
            xml->setAttribute(pfx + "_button_on_font_colour",
                              theme->button_on_font_colour.toString());
            xml->setAttribute(pfx + "_button_on_colour", theme->button_on_colour.toString());
            xml->setAttribute(pfx + "_button_off_font_colour",
                              theme->button_off_font_colour.toString());
            xml->setAttribute(pfx + "_button_off_colour", theme->button_off_colour.toString());

            xml->setAttribute(pfx + "_oszi_1", theme->oszi_1.toString());
            xml->setAttribute(pfx + "_oszi_2", theme->oszi_2.toString());
            xml->setAttribute(pfx + "_oszi_3", theme->oszi_3.toString());
        }
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
UiLookAndFeel::UiLookAndFeel() noexcept
    : mainwindow(nullptr),
      is_global_user_return(false, generate_param_name("LF", 0, "is_global_user_return"),
                            generate_short_human_name("LF", "is_global_user_return")),
      is_global_factory_return(false, generate_param_name("LF", 0, "is_global_factory_return"),
                               generate_short_human_name("LF", "is_global_factory_return")),
      is_global_program_return(false, generate_param_name("LF", 0, "is_global_program_return"),
                               generate_short_human_name("LF", "is_global_program_return")),
      is_global_undo_return(true, generate_param_name("LF", 0, "is_global_undo_return"),
                            generate_short_human_name("LF", "is_global_undo_return")),

      synth_data(nullptr),

      popup_smooth_Slider(new Slider("")), popup_linear_sensi_slider(new Slider("")),
      popup_rotary_sensi_slider(new Slider("")), popup_midi_snap_slider(new Slider("")),

      show_values_always(false, generate_param_name("LF", 1, "show_values_always"),
                         generate_short_human_name("LF", "show_values_always"))
{
#ifdef JUCE_DEBUG
    std::cout << "MONIQUE: init style" << std::endl;
#endif

    popup_smooth_Slider->addListener(this);
    popup_rotary_sensi_slider->addListener(this);
    popup_linear_sensi_slider->addListener(this);
    popup_midi_snap_slider->addListener(this);

    // initialise the standard set of colours..
    const uint32 textButtonColour = 0xffbbbbff;
    const uint32 textHighlightColour = 0x401111ee;
    const uint32 standardOutlineColour = 0xb2808080;

    static const uint32 standardColours[] = {
        TextButton::buttonColourId,
        0xff000000,
        TextButton::buttonOnColourId,
        0xff4444ff,
        TextButton::textColourOnId,
        0xff000000,
        TextButton::textColourOffId,
        0xff000000,

        ToggleButton::textColourId,
        0xff000000,

        TextEditor::backgroundColourId,
        0xffffffff,
        TextEditor::textColourId,
        colours.get_theme(COLOUR_THEMES::BG_THEME).area_font_colour.getARGB(),
        TextEditor::highlightColourId,
        0x66ff0000,
        TextEditor::highlightedTextColourId,
        0xffffffff,
        TextEditor::outlineColourId,
        0x00000000,
        TextEditor::focusedOutlineColourId,
        textButtonColour,
        TextEditor::shadowColourId,
        0x38000000,

        CaretComponent::caretColourId,
        0xff000000,

        Label::backgroundColourId,
        0x00000000,
        Label::textColourId,
        0xffff3b00,
        Label::outlineColourId,
        0x00000000,

        ScrollBar::backgroundColourId,
        0x00000000,
        ScrollBar::thumbColourId,
        0xffffffff,

        TreeView::linesColourId,
        0x4c000000,
        TreeView::backgroundColourId,
        0x00000000,
        TreeView::dragAndDropIndicatorColourId,
        0x80ff0000,
        TreeView::selectedItemBackgroundColourId,
        0x00000000,

        PopupMenu::backgroundColourId,
        0xffffffff,
        PopupMenu::textColourId,
        0xff000000,
        PopupMenu::headerTextColourId,
        0xff000000,
        PopupMenu::highlightedTextColourId,
        0xffffffff,
        PopupMenu::highlightedBackgroundColourId,
        0x991111aa,

        ComboBox::buttonColourId,
        0xffbbbbff,
        ComboBox::outlineColourId,
        0xffff0000,
        ComboBox::textColourId,
        0xffff0000,
        ComboBox::backgroundColourId,
        0xff000000,
        ComboBox::arrowColourId,
        0x99000000,

        PropertyComponent::backgroundColourId,
        0x66ffffff,
        PropertyComponent::labelTextColourId,
        0xffff3b00,

        TextPropertyComponent::backgroundColourId,
        0xffffffff,
        TextPropertyComponent::textColourId,
        0xffffffff,
        TextPropertyComponent::outlineColourId,
        standardOutlineColour,

        ListBox::backgroundColourId,
        0xffffffff,
        ListBox::outlineColourId,
        standardOutlineColour,
        ListBox::textColourId,
        0xff000000,

        Slider::backgroundColourId,
        0x00000000,
        Slider::thumbColourId,
        textButtonColour,
        Slider::trackColourId,
        0x7fffffff,
        Slider::rotarySliderFillColourId,
        0x00000000,
        Slider::rotarySliderOutlineColourId,
        0x66000000,
        Slider::textBoxTextColourId,
        Colour(Colours::yellowgreen).getARGB(),
        Slider::textBoxBackgroundColourId,
        0xff000000,
        Slider::textBoxHighlightColourId,
        textHighlightColour,
        Slider::textBoxOutlineColourId,
        0xffff3b00,

        ResizableWindow::backgroundColourId,
        0xff777777,
        // DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)

        AlertWindow::backgroundColourId,
        0xff161616,
        AlertWindow::textColourId,
        0xffffffff,
        AlertWindow::outlineColourId,
        0xffffffff,

        ProgressBar::backgroundColourId,
        0xffeeeeee,
        ProgressBar::foregroundColourId,
        0xffaaaaee,

        TooltipWindow::backgroundColourId,
        0xff000000,
        TooltipWindow::textColourId,
        0xffffff99,
        TooltipWindow::outlineColourId,
        0xffffff99,

        TabbedComponent::backgroundColourId,
        0x00000000,
        TabbedComponent::outlineColourId,
        0xff777777,
        TabbedButtonBar::tabOutlineColourId,
        0x80000000,
        TabbedButtonBar::frontOutlineColourId,
        0x90000000,

        Toolbar::backgroundColourId,
        0xfff6f8f9,
        Toolbar::separatorColourId,
        0x4c000000,
        Toolbar::buttonMouseOverBackgroundColourId,
        0x4c0000ff,
        Toolbar::buttonMouseDownBackgroundColourId,
        0x800000ff,
        Toolbar::labelTextColourId,
        0xffff3b00,
        Toolbar::editingModeOutlineColourId,
        0xffff0000,

        DrawableButton::textColourId,
        0xff000000,
        DrawableButton::textColourOnId,
        0xff000000,
        DrawableButton::backgroundColourId,
        0x00000000,
        DrawableButton::backgroundOnColourId,
        0xaabbbbff,

        HyperlinkButton::textColourId,
        0xcc1111ee,

        GroupComponent::outlineColourId,
        0x66000000,
        GroupComponent::textColourId,
        0xff000000,

        BubbleComponent::backgroundColourId,
        0xeeeeeebb,
        BubbleComponent::outlineColourId,
        0x77000000,

        DirectoryContentsDisplayComponent::highlightColourId,
        textHighlightColour,
        DirectoryContentsDisplayComponent::textColourId,
        0xff000000,

        0x1000440,
        /*LassoComponent::lassoFillColourId*/ 0x66dddddd,
        0x1000441,
        /*LassoComponent::lassoOutlineColourId*/ 0x99111111,

        0x1005000,
        /*MidiKeyboardComponent::whiteNoteColourId*/ 0xffffffff,
        0x1005001,
        /*MidiKeyboardComponent::blackNoteColourId*/ 0xff000000,
        0x1005002,
        /*MidiKeyboardComponent::keySeparatorLineColourId*/
        colours.get_theme(COLOUR_THEMES::BG_THEME).area_colour.getARGB(),
        0x1005003,
        /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/
        colours.get_theme(COLOUR_THEMES::KEYBOARD_THEME).button_on_colour.getARGB(),
        0x1005004,
        /*MidiKeyboardComponent::keyDownOverlayColourId*/
        colours.get_theme(COLOUR_THEMES::KEYBOARD_THEME).button_on_colour.withAlpha(0.5f).getARGB(),
        0x1005005,
        /*MidiKeyboardComponent::textLabelColourId*/ 0xff000000,
        0x1005006,
        /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/
        colours.get_theme(COLOUR_THEMES::BG_THEME).area_colour.getARGB(),
        0x1005007,
        /*MidiKeyboardComponent::upDownButtonArrowColourId*/
        colours.get_theme(COLOUR_THEMES::KEYBOARD_THEME).button_on_colour.withAlpha(0.5f).getARGB(),
        0x1005008,
        /*MidiKeyboardComponent::shadowColourId*/
        colours.get_theme(COLOUR_THEMES::BG_THEME).area_colour.getARGB(),

        0x1004500,
        /*CodeEditorComponent::backgroundColourId*/ 0xffffffff,
        0x1004502,
        /*CodeEditorComponent::highlightColourId*/ textHighlightColour,
        0x1004503,
        /*CodeEditorComponent::defaultTextColourId*/ 0xff000000,
        0x1004504,
        /*CodeEditorComponent::lineNumberBackgroundId*/ 0x44999999,
        0x1004505,
        /*CodeEditorComponent::lineNumberTextId*/ 0x44000000,

        0x1007000,
        /*ColourSelector::backgroundColourId*/ 0xff050505,
        0x1007001,
        /*ColourSelector::labelTextColourId*/ 0xff000000,

        0x100ad00,
        /*KeyMappingEditorComponent::backgroundColourId*/ 0x00000000,
        0x100ad01,
        /*KeyMappingEditorComponent::textColourId*/ 0xff000000,

        FileSearchPathListComponent::backgroundColourId,
        0xffffffff,

        FileChooserDialogBox::titleTextColourId,
        0xffff3b00,
    };

    show_values_always = false;
    midi_learn_comp = nullptr;

    for (int i = 0; i < numElementsInArray(standardColours); i += 2)
        setColour((int)standardColours[i], Colour((uint32)standardColours[i + 1]));

    defaultFont = Font(Typeface::createSystemTypefaceFor(BinaryData::LatoSemibold_ttf,
                                                         BinaryData::LatoSemibold_ttfSize))
                      .withHeight(15.0f);
    ScopedPointer<XmlElement> xml = XmlDocument::parse(BinaryData::DARK_mcol).release();
    colours.read_from(xml);
    // defaultFont =
    // Font(Typeface::createSystemTypefaceFor(BinaryData::Tahoma_ttf,BinaryData::Tahoma_ttfSize));
    // defaultFont =
    // Font(Typeface::createSystemTypefaceFor(BinaryData::Segoe,BinaryData::SegoeSize));
}
UiLookAndFeel::~UiLookAndFeel() noexcept {}

//==============================================================================
void UiLookAndFeel::drawButtonBackground(Graphics &g, Button &button,
                                         const Colour &backgroundColour, bool isMouseOverButton,
                                         bool isButtonDown)
{

    SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(button.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    const bool override_theme_colour =
        button.getProperties().getWithDefault(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, false);
    const float amp(button.getProperties().getWithDefault(VAR_INDEX_BUTTON_AMP, 0.0f));
    const bool is_toggled = button.getToggleState();
    const bool is_enabled = button.isEnabled();
    if (button.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    const bool is_midi_learn_mode = static_cast<Component *>(&button) == midi_learn_comp;

    // isMouseOverButton
    Colour color_1;
    if (is_midi_learn_mode)
    {
        color_1 =
            isButtonDown ? colours.midi_learn.darker(0.4f) : colours.midi_learn.brighter(0.25f);
    }
    else if (override_theme_colour)
    {
        color_1 = button.findColour(TextButton::buttonColourId);
    }
    else if (amp == COLOR_REPLACEMENTS::FORCE_BIT_RED)
    {
        color_1 = theme.button_off_colour.interpolatedWith(Colours::red, 0.2f);
    }
    else if (amp == COLOR_REPLACEMENTS::FORCE_RED)
    {
        color_1 = theme.button_on_colour.interpolatedWith(Colours::red, 0.9f);
    }
    else if (amp == VALUE_SLIDER_COLOUR)
    {
        color_1 = theme.value_slider_track_colour;
    }
    else if (amp == VALUE_SLIDER_2_COLOUR)
    {
        color_1 = theme.value_2_slider_track_colour;
    }
    else if (amp == MOD_SLIDER_COLOUR)
    {
        color_1 = theme.mod_slider_track_colour;
    }
    else if (amp == AREA_COLOUR)
    {
        color_1 = theme.area_colour;
    }
    else if (amp == AREA_FONT_COLOUR)
    {
        color_1 = theme.area_font_colour;
    }
    else if (amp == DISABLED_SLIDER_COLOUR)
    {
        color_1 = theme.disabled_track_colour;
    }
    else if (amp == SLIDER_BACKGROUND_COLOUR)
    {
        color_1 = theme.slider_bg_colour;
    }
    else if (amp == BUTTON_ON_COLOUR)
    {
        color_1 = theme.button_on_colour;
    }
    else if (amp == BUTTON_ON_FONT_COLOUR)
    {
        color_1 = theme.button_on_font_colour;
    }
    else if (amp == BUTTON_OFF_COLOUR)
    {
        color_1 = theme.button_off_colour;
    }
    else if (amp == BUTTON_OFF_FONT_COLOUR)
    {
        color_1 = theme.button_off_font_colour;
    }
    else if (amp == COLOR_REPLACEMENTS::USE_AREA_COLOUR)
    {
        color_1 = theme.area_colour;
    }
    else if (amp == COLOR_REPLACEMENTS::USE_AREA_TRANSCULENT)
    {
        color_1 = theme.area_colour.withAlpha(0.5f);
    }
    else if (amp == OSZI_1)
    {
        color_1 = theme.oszi_1;
    }
    else if (amp == OSZI_2)
    {
        color_1 = theme.oszi_2;
    }
    else if (amp == OSZI_3)
    {
        color_1 = theme.oszi_3;
    }
    else if (amp == 1)
    {
        color_1 = theme.button_on_colour;
    }
    else if (amp != 0)
    {
        color_1 =
            theme.button_on_colour.darker(1.0f - amp)
                .interpolatedWith(theme.button_off_colour, 1.0f - amp); // Colour(backgroundColour);
    }
    else if (is_toggled)
    {
        color_1 =
            theme.button_on_colour.darker(1.0f - amp)
                .interpolatedWith(theme.button_off_colour, 1.0f - amp); // Colour(backgroundColour);
    }
    else
    {
        color_1 = theme.button_off_colour;
    }

    if (isButtonDown)
    {
        color_1 = color_1.darker();
    }

    /*
    if( is_forced_off )
            {
                col = theme->button_off_colour.interpolatedWith(Colours::red,0.2f);
            }
            else if( is_forced_on )
            {
                col = theme->button_on_colour.interpolatedWith(Colours::red,0.5f);
            }
            else
            {
                col = top_parameter->get_value() == true ? theme->button_on_colour :
    theme->button_off_colour;
            }

    */

    /*
    else if( not button.isEnabled() )
    {
        color_1 =  Colour(backgroundColour).withAlpha(0.5f);
    }
    else
    {
        color_1 =  isButtonDown ? Colour(backgroundColour).darker (0.4f) : Colour(backgroundColour);
    }
    */

    {
        g.setColour(is_enabled ? color_1 : color_1.interpolatedWith(Colour(0x55333333), 0.3f));

        const bool flatOnLeft = button.isConnectedOnLeft();
        const float flatOnRight = button.isConnectedOnRight();
        const float flatOnTop = button.isConnectedOnTop();
        const float flatOnBottom = button.isConnectedOnBottom();
        Path highlight;
        highlight.addRoundedRectangle(1, 1, button.getWidth() - 2, button.getHeight() - 2, 4, 4,
                                      !(flatOnLeft || flatOnTop), !(flatOnRight || flatOnTop),
                                      !(flatOnLeft || flatOnBottom),
                                      !(flatOnRight || flatOnBottom));

        g.fillPath(highlight);
    }
}

Font UiLookAndFeel::getTextButtonFont(TextButton &button, int buttonHeigh)
{
    return defaultFont; // button.getFont();
}
#define FONT_SCALE 0.55f
#define IDENT_SCALE (1.0f - FONT_SCALE) * 0.5
void UiLookAndFeel::drawButtonText(Graphics &g, TextButton &button, bool /*isMouseOverButton*/,
                                   bool /*isButtonDown*/)
{
    const float height = button.getHeight();
    const float width = button.getWidth();
    const float fontHeight = FONT_SCALE * height;
    const float yIndent = IDENT_SCALE * height;
    const float cornerSize = jmin(height, width) * 0.2;
    const float leftIndent =
        jmin(fontHeight, 1.0f + cornerSize / (button.isConnectedOnLeft() ? 4 : 4));
    const float rightIndent =
        jmin(fontHeight, 1.0f + cornerSize / (button.isConnectedOnRight() ? 4 : 4));

    const SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(button.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    const bool override_theme_colour =
        button.getProperties().getWithDefault(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, false);
    const bool is_enabled = button.isEnabled();
    const float amp(button.getProperties().getWithDefault(VAR_INDEX_BUTTON_AMP, 0.0f));

    const bool is_toggled = amp != 0;
    Colour color_1;
    if (amp == COLOR_REPLACEMENTS::USE_AREA_COLOUR)
    {
        color_1 = theme.area_font_colour;
    }
    else if (amp == COLOR_REPLACEMENTS::USE_AREA_TRANSCULENT)
    {
        color_1 = theme.area_font_colour;
    }
    else if (amp == COLOUR_CODES::AREA_COLOUR)
    {
        color_1 = theme.area_font_colour;
    }
    else if (amp == COLOUR_CODES::AREA_FONT_COLOUR)
    {
        color_1 = theme.area_colour;
    }
    else if (amp == COLOUR_CODES::BUTTON_OFF_COLOUR)
    {
        color_1 = theme.button_off_font_colour;
    }
    else if (amp == COLOUR_CODES::BUTTON_ON_FONT_COLOUR)
    {
        color_1 = theme.button_on_colour;
    }
    else if (amp == COLOUR_CODES::BUTTON_OFF_FONT_COLOUR)
    {
        color_1 = theme.button_off_colour;
    }
    else if (amp == COLOUR_CODES::VALUE_SLIDER_COLOUR)
    {
        color_1 = theme.slider_bg_colour;
    }
    else if (amp == COLOUR_CODES::VALUE_SLIDER_2_COLOUR)
    {
        color_1 = theme.slider_bg_colour;
    }
    else if (amp == COLOUR_CODES::MOD_SLIDER_COLOUR)
    {
        color_1 = theme.slider_bg_colour;
    }
    else if (amp == COLOUR_CODES::DISABLED_SLIDER_COLOUR)
    {
        color_1 = theme.slider_bg_colour;
    }
    else if (amp == COLOUR_CODES::SLIDER_BACKGROUND_COLOUR)
    {
        color_1 = theme.value_slider_track_colour;
    }
    else
    {
        color_1 = override_theme_colour
                      ? button.findColour(TextButton::buttonColourId).contrasting(1)
                  : is_toggled ? theme.button_on_font_colour
                               : theme.button_off_font_colour;
    }
    g.setFont(defaultFont.withHeight(fontHeight));
    g.setColour(is_enabled ? color_1 : color_1.interpolatedWith(Colour(0x55333333), 0.3f));
    g.drawText(
        button.getButtonText(),
        Rectangle<float>(leftIndent, yIndent, (width - leftIndent - rightIndent), fontHeight),
        Justification::centred, false);
}

void UiLookAndFeel::drawTickBox(Graphics &g, Component &component, float x, float y, float w,
                                float h, const bool ticked, const bool isEnabled,
                                const bool isMouseOverButton, const bool isButtonDown)
{
    SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(component.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    if (component.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    g.setColour(ticked ? theme.button_on_colour : theme.area_colour);

    g.fillRect(int(x) - 1, int(y) - 1, int(w), int(h));

    g.setColour(ticked ? theme.button_off_colour : theme.button_on_colour);
    g.drawRect(int(x) - 1, int(y) - 1, int(w), int(h), 1);
}

void UiLookAndFeel::drawToggleButton(Graphics &g, ToggleButton &button, bool isMouseOverButton,
                                     bool isButtonDown)
{
    // if( CAN_OPAQUE and button.isOpaque() )
    //    g.fillAll (colours.bg);

    float prop_h = 1.f / 25.f * button.getHeight();
    float top = prop_h * 4.f;
    float rect_size = button.getHeight() - top * 2;
    drawTickBox(g, button, (button.getWidth() - rect_size) / 2, top, rect_size, rect_size,
                button.getToggleState(), button.isEnabled(), isMouseOverButton, isButtonDown);
}

//==============================================================================
AlertWindow *UiLookAndFeel::createAlertWindow(const String &title, const String &message,
                                              const String &button1, const String &button2,
                                              const String &button3,
                                              AlertWindow::AlertIconType iconType, int numButtons,
                                              Component *associatedComponent)
{
    AlertWindow *aw = new AlertWindow(title, message, iconType, associatedComponent);

    if (numButtons == 1)
    {
        aw->addButton(button1, 0, KeyPress(KeyPress::escapeKey), KeyPress(KeyPress::returnKey));
    }
    else
    {
        const KeyPress button1ShortCut((int)CharacterFunctions::toLowerCase(button1[0]), 0, 0);
        KeyPress button2ShortCut((int)CharacterFunctions::toLowerCase(button2[0]), 0, 0);
        if (button1ShortCut == button2ShortCut)
            button2ShortCut = KeyPress();

        if (numButtons == 2)
        {
            aw->addButton(button1, 1, KeyPress(KeyPress::returnKey), button1ShortCut);
            aw->addButton(button2, 0, KeyPress(KeyPress::escapeKey), button2ShortCut);
        }
        else if (numButtons == 3)
        {
            aw->addButton(button1, 1, button1ShortCut);
            aw->addButton(button2, 2, button2ShortCut);
            aw->addButton(button3, 0, KeyPress(KeyPress::escapeKey));
        }
    }
    for (int i = 0; i != aw->getNumChildComponents(); ++i)
    {
        if (Button *button = dynamic_cast<Button *>(aw->getChildComponent(i)))
        {
            button->setColour(TextButton::buttonColourId, Colours::white);
        }
    }

    return aw;
}

void UiLookAndFeel::drawAlertBox(Graphics &g, AlertWindow &alert, const Rectangle<int> &textArea,
                                 TextLayout &textLayout)
{
    // g.fillAll (alert.findColour (AlertWindow::backgroundColourId));

    g.fillRoundedRectangle(0, 0, alert.getWidth(), alert.getHeight(), 10);

    int iconSpaceUsed = 0;

    const int iconWidth = 50;
    int iconSize = jmin(iconWidth + 50, alert.getHeight() + 20);

    if (alert.containsAnyExtraComponents() || alert.getNumButtons() > 2)
        iconSize = jmin(iconSize, textArea.getHeight() + 50);

    const Rectangle<int> iconRect(iconSize / -10, iconSize / -10, iconSize, iconSize);

    if (alert.getAlertType() != AlertWindow::NoIcon)
    {
        Path icon;
        uint32 colour;
        char character;

        if (alert.getAlertType() == AlertWindow::WarningIcon)
        {
            colour = 0x55ff5555;
            character = '!';

            icon.addTriangle(iconRect.getX() + iconRect.getWidth() * 0.5f, (float)iconRect.getY(),
                             (float)iconRect.getRight(), (float)iconRect.getBottom(),
                             (float)iconRect.getX(), (float)iconRect.getBottom());

            icon = icon.createPathWithRoundedCorners(5.0f);
        }
        else
        {
            colour = alert.getAlertType() == AlertWindow::InfoIcon ? (uint32)0x605555ff
                                                                   : (uint32)0x40b69900;
            character = alert.getAlertType() == AlertWindow::InfoIcon ? 'i' : '?';

            icon.addEllipse((float)iconRect.getX(), (float)iconRect.getY(),
                            (float)iconRect.getWidth(), (float)iconRect.getHeight());
        }

        GlyphArrangement ga;
        ga.addFittedText(Font(iconRect.getHeight() * 0.9f, Font::bold),
                         String::charToString((juce_wchar)(uint8)character), (float)iconRect.getX(),
                         (float)iconRect.getY(), (float)iconRect.getWidth(),
                         (float)iconRect.getHeight(), Justification::centred, false);
        ga.createPath(icon);

        icon.setUsingNonZeroWinding(false);
        g.setColour(Colour(colour));
        g.fillPath(icon);

        iconSpaceUsed = iconWidth;
    }

    g.setColour(alert.findColour(AlertWindow::textColourId));

    textLayout.draw(g, Rectangle<int>(textArea.getX() + iconSpaceUsed, textArea.getY(),
                                      textArea.getWidth() - iconSpaceUsed, textArea.getHeight())
                           .toFloat());

    g.setColour(alert.findColour(AlertWindow::outlineColourId));
    g.drawRect(0, 0, alert.getWidth(), alert.getHeight(), 2);
}

int UiLookAndFeel::getAlertBoxWindowFlags()
{
    return ComponentPeer::windowAppearsOnTaskbar | ComponentPeer::windowHasDropShadow;
}

int UiLookAndFeel::getAlertWindowButtonHeight()
{
    return int(1.f / 900 *
               Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight() * 30);
}

Font UiLookAndFeel::getAlertWindowMessageFont()
{
    return defaultFont.withHeight(
        1.f / 900 * Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight() *
        20);
}

Font UiLookAndFeel::getAlertWindowFont()
{
    return defaultFont.withHeight(
        (1.f / 900 *
         Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight() * 17));
}

//==============================================================================
Font UiLookAndFeel::getPopupMenuFont() { return defaultFont; }

void UiLookAndFeel::getIdealPopupMenuItemSize(const String &text, const bool isSeparator,
                                              int standardMenuItemHeight, int &idealWidth,
                                              int &idealHeight)
{
    if (isSeparator)
    {
        idealWidth = 50;
        idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight / 2 : 10;
    }
    else
    {
        Font font(getPopupMenuFont());

        if (standardMenuItemHeight > 0 && font.getHeight() > standardMenuItemHeight / 1.3f)
            font.setHeight(standardMenuItemHeight / 1.3f);

        idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight
                                                 : roundToInt(font.getHeight() * 1.3f);
        idealWidth = font.getStringWidth(text) + idealHeight * 2;
    }
}

void UiLookAndFeel::drawPopupMenuBackground(Graphics &g, int width, int height)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    const Colour background(theme.button_off_colour);

    g.fillAll(background);
    g.setColour(background.overlaidWith(theme.area_font_colour.withAlpha(0.07f)));

    for (int i = 0; i < height; i += 3)
        g.fillRect(0, i, width, 1);

#if !JUCE_MAC
    g.setColour(findColour(PopupMenu::textColourId).withAlpha(0.6f));
    g.drawRect(0, 0, width, height);
#endif
}

void UiLookAndFeel::drawPopupMenuUpDownArrow(Graphics &g, int width, int height,
                                             bool isScrollUpArrow)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    const Colour background(theme.button_on_colour);

    g.setGradientFill(ColourGradient(background, 0.0f, height * 0.5f, background.withAlpha(0.0f),
                                     0.0f, isScrollUpArrow ? ((float)height) : 0.0f, false));

    g.fillRect(0, 0, width, height);

    const float hw = width * 0.5f;
    const float arrowW = height * 0.3f;
    const float y1 = height * (isScrollUpArrow ? 0.6f : 0.3f);
    const float y2 = height * (isScrollUpArrow ? 0.3f : 0.6f);

    Path p;
    p.addTriangle(hw - arrowW, y1, hw + arrowW, y1, hw, y2);

    g.setColour(theme.button_on_font_colour.withAlpha(0.8f));
    g.fillPath(p);
}

void UiLookAndFeel::drawPopupMenuItem(Graphics &g, const Rectangle<int> &area,
                                      const bool isSeparator, const bool isActive,
                                      const bool isHighlighted, const bool isTicked,
                                      const bool hasSubMenu, const String &text,
                                      const String &shortcutKeyText, const Drawable *icon,
                                      const Colour *const textColourToUse)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    if (isSeparator)
    {
        Rectangle<int> r(area.reduced(5, 0));
        r.removeFromTop(r.getHeight() / 2 - 1);

        g.setColour(Colour(0x33000000));
        g.fillRect(r.removeFromTop(1));

        g.setColour(Colour(0x66ffffff));
        g.fillRect(r.removeFromTop(1));
    }
    else
    {
        Rectangle<int> r(area.reduced(1));

        if (isHighlighted)
        {
            g.setColour(theme.button_on_colour);
            g.fillRect(r);

            g.setColour(theme.button_on_font_colour);
        }
        else if (isTicked)
        {
            g.setColour(theme.button_on_colour.withAlpha(0.7f));
            g.fillRect(r);

            g.setColour(theme.button_on_font_colour);
        }
        else
        {
            g.setColour(theme.button_off_font_colour);
        }

        if (!isActive)
            g.setOpacity(0.3f);

        Font font(getPopupMenuFont());

        const float maxFontHeight = area.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight(maxFontHeight);

        g.setFont(font);

        Rectangle<float> iconArea(r.removeFromLeft((r.getHeight() * 5) / 4).reduced(3).toFloat());

        if (icon != nullptr)
        {
            icon->drawWithin(g, iconArea,
                             RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                             1.0f);
        }
        else if (isTicked)
        {
            const Path tick(getTickShape(1.0f));
            g.fillPath(tick, tick.getTransformToScaleToFit(iconArea, true));
        }

        if (hasSubMenu)
        {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();

            const float x = (float)r.removeFromRight((int)arrowH).getX();
            const float halfH = (float)r.getCentreY();

            Path p;
            p.addTriangle(x, halfH - arrowH * 0.5f, x, halfH + arrowH * 0.5f, x + arrowH * 0.6f,
                          halfH);

            g.fillPath(p);
        }

        r.removeFromRight(3);
        g.drawFittedText(text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            Font f2(font);
            f2.setHeight(f2.getHeight() * 0.75f);
            f2.setHorizontalScale(0.95f);
            g.setFont(f2);

            g.drawText(shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}
void UiLookAndFeel::drawPopupMenuSectionHeader(Graphics &g, const Rectangle<int> &area,
                                               const String &sectionName)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    g.setFont(getPopupMenuFont().boldened());
    g.setColour(theme.button_on_colour);

    g.drawFittedText(sectionName, area.getX() + 12, area.getY(), area.getWidth() - 16,
                     (int)(area.getHeight() * 0.8f), Justification::bottomLeft, 1);
}
//==============================================================================
void UiLookAndFeel::fillTextEditorBackground(Graphics &g, int /*width*/, int /*height*/,
                                             TextEditor &textEditor)
{
    const bool override_theme_colour =
        textEditor.getProperties().getWithDefault(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, false);

    if (override_theme_colour)
    {
        g.fillAll(colours.get_theme(BG_THEME).area_colour);
    }
    else
    {
        g.fillAll(textEditor.findColour(TextEditor::backgroundColourId));
    }
}

void UiLookAndFeel::drawTextEditorOutline(Graphics &g, int width, int height,
                                          TextEditor &textEditor)
{
    /*
    if (textEditor.isEnabled())
    {
        if (textEditor.hasKeyboardFocus (true) && ! textEditor.isReadOnly())
        {
            g.setColour (textEditor.findColour (TextEditor::focusedOutlineColourId));
            g.drawRoundedRectangle (0, 0, width, height, 1, 1.5);

            g.setOpacity (1.0f);
            const Colour shadowColour (textEditor.findColour
    (TextEditor::shadowColourId).withMultipliedAlpha (0.75f)); drawBevel (g, 0, 0, width, height, 3,
    shadowColour, shadowColour);
        }
        else
        {
            g.setColour (textEditor.findColour (TextEditor::outlineColourId));
            g.drawRect (0, 0, width, height);

            g.setOpacity (1.0f);
            const Colour shadowColour (textEditor.findColour (TextEditor::shadowColourId));
            drawBevel (g, 0, 0, width, height, 3, shadowColour, shadowColour);
        }
    }
    */
}

CaretComponent *UiLookAndFeel::createCaretComponent(Component *keyFocusOwner)
{
    return new CaretComponent(keyFocusOwner);
}

//==============================================================================
void UiLookAndFeel::drawComboBox(Graphics &g, int width, int height, const bool isButtonDown,
                                 int buttonX, int buttonY, int buttonW, int buttonH, ComboBox &box)
{
    SectionTheme &theme = colours.get_theme(static_cast<COLOUR_THEMES>(int(
        box.getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    const Colour &user_colour = box.findColour(ComboBox::backgroundColourId);
    if (box.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    const float outlineThickness = 1; // box.isEnabled() ? (isButtonDown ? 1.2f : 0.8f) : 0.3f;

    Colour color_1 = user_colour.getARGB() != 0xff000000
                         ? user_colour
                         : theme.button_off_colour; // 0xff000000 default col

    g.setColour(color_1);
    g.fillRoundedRectangle(1, 1, width - 2, height - 2, 4);

    drawGlassLozenge(g, buttonX + outlineThickness, buttonY + outlineThickness,
                     buttonW - outlineThickness * 2.0f, buttonH - outlineThickness * 2.0f,
                     Colour(0xff333333), outlineThickness, -1.0f, true, true, true, true);

    if (box.isEnabled() && box.hasKeyboardFocus(false))
    {
        g.setColour(theme.button_on_colour);
        g.drawRoundedRectangle(1, 1, width - 2, height - 2, 4, 1.5);
    }
    else
    {
        g.setColour(Colour(0xff333333));
        g.drawRoundedRectangle(1, 1, width - 2, height - 2, 4, 1.5);
    }

    if (box.isEnabled())
    {
        const float arrowX = 0.3f;
        const float arrowH = 0.2f;

        Path p;
        p.addTriangle(buttonX + buttonW * 0.5f, buttonY + buttonH * (0.45f - arrowH),
                      buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.45f,
                      buttonX + buttonW * arrowX, buttonY + buttonH * 0.45f);

        p.addTriangle(buttonX + buttonW * 0.5f, buttonY + buttonH * (0.55f + arrowH),
                      buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.55f,
                      buttonX + buttonW * arrowX, buttonY + buttonH * 0.55f);

        g.setColour(theme.button_on_font_colour);
        g.fillPath(p);
    }
}

Font UiLookAndFeel::getComboBoxFont(ComboBox &box)
{
    return defaultFont.withHeight(jmin(15.0f, box.getHeight() * 0.85f));
}

Label *UiLookAndFeel::createComboBoxTextBox(ComboBox &)
{
    Label *label = new Label(String(), String());
    label->getProperties().set(VAR_INDEX_BUTTON_AMP, BUTTON_ON_COLOUR);
    return label;
}

void UiLookAndFeel::positionComboBoxText(ComboBox &box, Label &label)
{
    label.setBounds(1, 1, box.getWidth() + 3 - box.getHeight(), box.getHeight() - 2);

    label.setFont(getComboBoxFont(box));
}

//==============================================================================
Font UiLookAndFeel::getLabelFont(Label &label) { return defaultFont; }

void UiLookAndFeel::drawLabel(Graphics &g, Label &label)
{
    SectionTheme &theme = colours.get_theme(static_cast<COLOUR_THEMES>(int(
        label.getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME))));
    const bool is_inverted =
        label.getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME_INVERTED, false);
    const float amp = label.getProperties().getWithDefault(VAR_INDEX_BUTTON_AMP, 0);
    if (label.isOpaque())
    {
        g.fillAll(not is_inverted ? theme.area_colour : theme.area_font_colour);
    }

    if (not label.isBeingEdited())
    {
        const float height = label.getHeight();
        const float width = label.getWidth();
        const float fontHeight = FONT_SCALE * height;
        const float yIndent = IDENT_SCALE * height;
        const float cornerSize = jmin(height, width) * 0.2;
        const float leftIndent = jmin(fontHeight, 1.0f + cornerSize / 4);
        const float rightIndent = jmin(fontHeight, 1.0f + cornerSize / 4);

        g.setFont(defaultFont.withHeight(fontHeight));
        Colour col;
        if (amp != 0)
        {
            col = theme.get_color(static_cast<COLOUR_CODES>(int(amp)));
        }
        else
        {
            col = theme.area_font_colour;
        }
        g.setColour(is_inverted ? theme.area_colour : col);
        g.drawText(
            label.getText(),
            Rectangle<float>(leftIndent, yIndent, (width - leftIndent - rightIndent), fontHeight),
            Justification::centred, false);
    }
}

//==============================================================================
void UiLookAndFeel::drawLinearSliderBackground(Graphics &g, int x, int y, int width, int height,
                                               float sliderPos, float minSliderPos,
                                               float maxSliderPos,
                                               const Slider::SliderStyle /*style*/, Slider &slider)
{
    SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(slider.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME))));
    if (slider.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    const bool is_midi_learn_mode = static_cast<Component *>(&slider) == midi_learn_comp;

    Colour col;
    const bool is_enabled = slider.isEnabled();
    const int slider_type =
        slider.getProperties().getWithDefault(VAR_INDEX_SLIDER_TYPE, VALUE_SLIDER);
    if (is_midi_learn_mode)
    {
        col = colours.midi_learn;
    }
    else
    {
        if (!is_enabled)
            col = theme.disabled_track_colour;
        else if (slider_type == VALUE_SLIDER)
            col = theme.value_slider_track_colour;
        else if (slider_type == VALUE_SLIDER_2)
            col = theme.value_2_slider_track_colour;
        else
            col = theme.mod_slider_track_colour;
    }

    const float sliderRadius = (float)(getSliderThumbRadius(slider) - 2);
    Path indent;
    if (slider.isHorizontal())
    {
        const float iy = y + height * 0.5f - sliderRadius * 0.75;
        const float ih = sliderRadius * 1.5;

        {
            g.setColour(theme.slider_bg_colour);
            indent.addRoundedRectangle(2, iy, slider.getWidth() - 4, ih - 4, 2);
            g.fillPath(indent);
            g.strokePath(indent, PathStrokeType(2.5f));
            indent.clear();
        }

        {
            g.setColour(col);
            float width = slider.getWidth() - 4;
            const float value = slider.getValue();
            float scale = value > 0 ? 0.02 : 0;
            const float real_scale = 1.0f / slider.getMaximum() * slider.getValue();
            if (real_scale > 0.02)
            {
                scale = real_scale;
            }

            float slider_pos = width * scale;
            indent.addRoundedRectangle(2, iy, slider_pos, ih - 4, 2);

            g.fillPath(indent);
        }

        g.fillPath(indent);
        g.strokePath(indent, PathStrokeType(2.5f));

        // g.setColour (Colour (colours.bg_lines));
        // g.setColour (Colour(0xff777777).darker(7.8));
        // g.strokePath (indent, PathStrokeType (1.0f));
    }
    else
    {
        const float ix = x + width * 0.5f - sliderRadius * 0.75;
        const float iw = width * 0.16;

        {
            g.setColour(theme.slider_bg_colour);
            indent.addRoundedRectangle(ix, 2, iw, slider.getHeight() - 4, 2);
            g.fillPath(indent);
            g.strokePath(indent, PathStrokeType(2.5f));
            indent.clear();
        }

        {
            g.setColour(col);
            float height = slider.getHeight() - 4;
            const float value = slider.getValue();
            float scale = value > 0 ? 0.02 : 0;
            const float real_scale = 1.0f / slider.getMaximum() * slider.getValue();
            if (real_scale > 0.02)
            {
                scale = real_scale;
            }

            float slider_pos = height * scale;
            indent.addRoundedRectangle(ix, height - slider_pos + 2, iw, slider_pos, 2);

            g.fillPath(indent);
        }
    }
}

void UiLookAndFeel::drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height,
                                          float sliderPos, float minSliderPos, float maxSliderPos,
                                          const Slider::SliderStyle style, Slider &slider)
{
    /*

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

                     */
}

void UiLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height,
                                     float sliderPos, float minSliderPos, float maxSliderPos,
                                     const Slider::SliderStyle style, Slider &slider)
{
    SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(slider.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    if (slider.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
    {
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

        Colour baseColour(LookAndFeelHelpers::createBaseColour(
            slider.findColour(Slider::thumbColourId)
                .withMultipliedSaturation(slider.isEnabled() ? 1.0f : 0.5f),
            false, isMouseOver, isMouseOver || slider.isMouseButtonDown()));

        drawShinyButtonShape(
            g, (float)x, style == Slider::LinearBarVertical ? sliderPos : (float)y,
            style == Slider::LinearBarVertical ? (float)width : (sliderPos - x),
            style == Slider::LinearBarVertical ? (height - sliderPos) : (float)height, 0.0f,
            baseColour, slider.isEnabled() ? 0.9f : 0.3f, true, true, true, true);
    }
    else
    {
        drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos,
                                   style, slider);
        drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style,
                              slider);
    }
}

int UiLookAndFeel::getSliderThumbRadius(Slider &slider)
{
    return jmin(5, slider.getHeight() / 2, slider.getWidth() / 2) + 2;
}

void UiLookAndFeel::drawRotarySlider(Graphics &g, int x, int y, int width, int height,
                                     float sliderPos, const float rotaryStartAngle,
                                     const float rotaryEndAngle, Slider &slider)
{
    NamedValueSet &properties = slider.getProperties();
    const SectionTheme &theme = colours.get_theme(static_cast<COLOUR_THEMES>(
        int(properties.getWithDefault(VAR_INDEX_COLOUR_THEME, DUMMY_THEME))));
    const int slider_type =
        slider.getProperties().getWithDefault(VAR_INDEX_SLIDER_TYPE, VALUE_SLIDER);
    if (slider.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    float slider_value = slider.getValue();
    float rotaryStartAngle_ = rotaryStartAngle;
    float rotaryEndAngle_ = rotaryEndAngle;

    const bool is_midi_learn_mode = static_cast<Component *>(&slider) == midi_learn_comp;
    if (slider.getMinimum() < 0)
    {
        if (slider_type == MODULATION_SLIDER)
        {
            rotaryEndAngle_ = juce::MathConstants<float>::pi * 0.8f;
            rotaryStartAngle_ = 0;
            sliderPos = slider_value * rotaryEndAngle_ / 2.5f;
        }
        else
        {
            rotaryStartAngle_ = 0;
            rotaryEndAngle_ = juce::MathConstants<float>::pi * 0.8f;
            sliderPos = (1.0f / slider.getMaximum() * slider_value) * rotaryEndAngle_ / 2.5f;
        }
    }

    float radius = jmin(width * 0.5f, height * 0.5f) - 2.0f;
    float centreX = x + width * 0.5f;
    float centreY = y + height * 0.5f;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float rw = radius * 2.0f;
    float angle = rotaryStartAngle_ + sliderPos * (rotaryEndAngle_ - rotaryStartAngle_);

    Colour SliderCol;
    if (is_midi_learn_mode)
    {
        SliderCol = colours.midi_learn;
    }
    else
    {
        if (!slider.isEnabled())
        {
            SliderCol = theme.disabled_track_colour;
        }
        else if (slider_type == VALUE_SLIDER)
        {
            SliderCol = theme.value_slider_track_colour;
        }
        else if (slider_type == VALUE_SLIDER_2)
        {
            SliderCol = theme.value_2_slider_track_colour;
        }
        else
        {
            SliderCol = theme.mod_slider_track_colour;
        }
    }

#define THICKNESS 0.79f /* (1.f/40*slider.getWidth()); // 0.7f; */

    {
        Path filledArc;
        if (slider.isOpaque())
        {
            filledArc.addPieSegment(rx, ry, rw, rw, 0, juce::MathConstants<double>::pi * 2,
                                    THICKNESS);

            // g.setColour(colours.bg.withAlpha(0.2f));
            // g.setColour (SliderCol.darker (5.f).withAlpha(0.5f));
            g.setColour(theme.slider_bg_colour); //.interpolatedWith(SliderCol.darker(0.8),0.2));
            g.fillPath(filledArc);
            g.strokePath(filledArc, PathStrokeType(1.5f));
            // g.setColour(Colour(0xff444444).darker(7.8));
            //  g.strokePath(filledArc,PathStrokeType(1.f));
        }

        filledArc.clear();
        // if( sliderPos != 0 )
        {
            if (slider_type != VALUE_SLIDER)
            {
                float _radius = radius * 0.75;
                float rx = centreX - _radius;
                float ry = centreY - _radius;
                float rw = _radius * 2.0f;
                filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle_, angle, THICKNESS);
                g.setColour(theme.slider_bg_colour);
                g.strokePath(filledArc, PathStrokeType(1.5f));
            }
            else
            {
                filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle_, angle, THICKNESS);
                g.setColour(theme.slider_bg_colour);
                g.strokePath(filledArc, PathStrokeType(2.5f));
            }

            {
                g.setColour(SliderCol);
                g.fillPath(filledArc);
            }
        }
    }

    // DRAW LABEL, WAVE OR CENTER TRIANGLE
    {
        const int slider_label_style = slider.getProperties().getWithDefault(
            VAR_INDEX_SLIDER_LABEL_STYLE, SLIDER_LABEL_STYLES::DONT_SHOW_TEXT);
        if (slider_label_style == SLIDER_LABEL_STYLES::SHOW_MIDDLE_TEXT_BOX)
        {
            String value_to_paint = slider.getProperties().getWithDefault(
                VAR_INDEX_VALUE_TO_SHOW, String(slider.getValue()));
            const String suffix = value_to_paint.fromFirstOccurrenceOf("@", false, true);
            value_to_paint = value_to_paint.upToFirstOccurrenceOf("@", false, true);

            if (suffix == "wav")
            {
                const float label_x_ident = float(width) / 3.2;
                const float label_y_ident = float(height) / 3.2;
                const float label_h = float(height) - label_y_ident * 2;
                const float label_w = float(width) - label_x_ident * 2;
                float value_as_float = value_to_paint.getFloatValue();

                Path wave_path;
                if (value_as_float <= 1 and value_as_float >= 0)
                {
                    float square_weight = value_as_float;
                    for (int i = 0; i < int(label_w * 4); ++i)
                    {
                        float value_sin = std::sin((1.0f / float(label_w * 4) * i) *
                                                   (juce::MathConstants<float>::pi * 2));
                        float value_square;
                        if (i < 1)
                        {
                            value_square = 0;
                        }
                        else if (i == label_w * 4 - 1)
                        {
                            value_square = 0;
                        }
                        else
                            value_square = value_sin >= 0 ? 1 : -1;

                        float mix =
                            value_sin * (1.0f - square_weight) + value_square * square_weight;

                        float x = label_x_ident + float(i) / 4;
                        float y = (label_y_ident + label_h) - label_h * (mix + 1) * 0.5;

                        if (i == 0)
                            wave_path.startNewSubPath(x, y);
                        else
                            wave_path.lineTo(x, y);
                    }
                    wave_path.lineTo(label_x_ident + label_w, label_y_ident + label_h * 0.5);
                }
                else if (value_as_float <= 2 and value_as_float >= 1)
                {
                    float saw_weight = value_as_float - 1;
                    for (int i = 0; i < int(label_w * 4); ++i)
                    {
                        float value_sin = std::sin((1.0f / float(label_w * 4) * i) *
                                                   (juce::MathConstants<float>::pi * 2));
                        float value_square;
                        if (i < 1)
                        {
                            value_square = 0;
                        }
                        else if (i == label_w * 4 - 1)
                        {
                            value_square = 0;
                        }
                        else
                            value_square = value_sin >= 0 ? 1 : -1;

                        float value_saw;
                        if (i < 1)
                        {
                            value_saw = 0;
                        }
                        else if (i == label_w * 4 - 1)
                        {
                            value_saw = 0;
                        }
                        else
                            value_saw = ((1.0f / (label_w * 4) * i) * 2 - 1) * -1;

                        float mix = value_square * (1.0f - saw_weight) + value_saw * saw_weight;

                        float x = label_x_ident + float(i) / 4;
                        float y = (label_y_ident + label_h) - label_h * (mix + 1) * 0.5;

                        if (i == 0)
                            wave_path.startNewSubPath(x, y);
                        else
                            wave_path.lineTo(x, y);
                    }
                    wave_path.lineTo(label_x_ident + label_w, label_y_ident + label_h * 0.5);
                }
                else if (value_as_float <= 3 and value_as_float >= 2)
                {
                    float rand_weight = value_as_float - 2;
                    for (int i = 0; i < int(label_w * 4); ++i)
                    {
                        float value_saw;
                        if (i < 1)
                        {
                            value_saw = 0;
                        }
                        else if (i == label_w * 4 - 1)
                        {
                            value_saw = 0;
                        }
                        else
                            value_saw = ((1.0f / (label_w * 4) * i) * 2 - 1) * -1;

                        float mix =
                            value_saw * (1.0f - rand_weight) +
                            ((Random::getSystemRandom().nextFloat() - 0.5) * 2) * rand_weight;

                        float x = label_x_ident + float(i) / 4;
                        float y = (label_y_ident + label_h) - label_h * (mix + 1) * 0.5;

                        if (i == 0)
                            wave_path.startNewSubPath(x, y);
                        else
                            wave_path.lineTo(x, y);
                    }
                    wave_path.lineTo(label_x_ident + label_w, label_y_ident + label_h * 0.5);
                }

                g.setColour(SliderCol);
                g.strokePath(wave_path, PathStrokeType(1.f));
                // g.setColour( SliderCol.withAlpha(0.6f) );
                // g.strokePath(wave_path,PathStrokeType(3.0f));
            }
            // DRAW VALUE
            else
            {
                const float label_x_ident = float(width) / 4;
                const float label_y_ident = float(height) / 3.25;
                const float label_h = float(height) - label_y_ident * 2;
                const float label_w = float(width) - label_x_ident * 2;

                Path text_path;
                GlyphArrangement glyphs;
                glyphs.addFittedText(defaultFont.withHeight(label_h), value_to_paint, label_x_ident,
                                     label_y_ident, label_w, label_h, Justification::centred, 1,
                                     0.5f);

                glyphs.createPath(text_path);
                g.fillPath(text_path);

                // DRAW SUFFIX
                if (suffix != "" && value_to_paint != "0")
                {
                    // SPECIAL FOR SMALL #
                    float font_height = 0.3;
                    if (suffix == "#")
                        font_height += 0.1;

                    int left_right_ident = width / 2.5;
                    text_path.clear();
                    glyphs.clear();
                    glyphs.addFittedText(defaultFont.withHeight(font_height * height), suffix,
                                         left_right_ident, height - height / 4,
                                         width - left_right_ident * 2, height / 4,
                                         Justification::centred, 1, 0.5f);
                    glyphs.createPath(text_path);
                    g.fillPath(text_path);
                }
            }
        }
    }
}

Font UiLookAndFeel::getSliderPopupFont(Slider &) { return defaultFont.withHeight(17.0f); }
int UiLookAndFeel::getSliderPopupPlacement(Slider &)
{
    return BubbleComponent::above | BubbleComponent::below | BubbleComponent::left |
           BubbleComponent::right;
}
PopupMenu *UiLookAndFeel::getCustomPopupMenu(Slider *slider_)
{
    PopupMenu *menu = new PopupMenu();
    menu->setLookAndFeel(this);

    {
        // RETURN VALUES
        {
            menu->addSeparator();
            menu->addSectionHeader("RESTORE VALUES");
            menu->addItem(1, TRANS("User"), true, false);
            menu->addItem(2, TRANS("Factory Default"), true, false);
            menu->addItem(3, TRANS("State On Program"), true, false);
            menu->addItem(4, TRANS("Undo"), true, false);

            menu->addSeparator();
            menu->addSectionHeader("UPDATE VALUES");
            menu->addItem(5, TRANS("Set new User Value"), true, false);

            menu->addSeparator();
            menu->addSectionHeader("GLOBAL DOUBLE CLICK RETURN MODE");
            menu->addItem(6, TRANS("User"), true, is_global_user_return);
            menu->addItem(7, TRANS("Factory Default"), true, is_global_factory_return);
            menu->addItem(8, TRANS("State On Program"), true, is_global_program_return);
            menu->addItem(9, TRANS("Swap (toggle last double click value and current value)"), true,
                          is_global_undo_return);
        }

        menu->addSeparator();
        menu->addSeparator();

        // INPUT SMOOTHING
        {
            menu->addSeparator();
            menu->addSectionHeader("Global User Input Smoothing");
            popup_smooth_Slider->setRange(1, 1000, 1);
            popup_smooth_Slider->setSliderStyle(Slider::LinearHorizontal);
            popup_smooth_Slider->setTextBoxStyle(Slider::NoTextBox, true, 70, 20);
            popup_smooth_Slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
            if (synth_data)
            {
                popup_smooth_Slider->setValue(int(synth_data->glide_motor_time),
                                              dontSendNotification);
            }

            {
                static bool fix_oss_port_issue = false;
                jassert(fix_oss_port_issue);
                fix_oss_port_issue = true;
            }
            /*
            menu->addCustomItem (10,
                                 popup_smooth_Slider,
                                 150, 30,
                                 false );
                                 */
        }

        // SLIDER HANDLING
        {
            menu->addSeparator();
            slider_menu = new PopupMenu();
            // slider_menu->addSectionHeader("GLOBAL SLIDER HANDLING");

            // LINEAR
            {
                slider_menu->addSeparator();
                slider_menu->addSectionHeader("LINEAR SLIDER HANDLING");
                slider_menu->addItem(14, TRANS("Use only Rotary Sliders"), true,
                                     synth_data->only_use_rotary_sliders);
                slider_menu->addItem(11, TRANS("Velocity-Sensitive Mode (Shortcut: CTRL+drag)"),
                                     true and not synth_data->only_use_rotary_sliders,
                                     synth_data->is_linear_sliders_velocity_mode);

                slider_menu->addSeparator();
                slider_menu->addSectionHeader("Linear Velocity Acceleration");
                popup_linear_sensi_slider->setEnabled(not synth_data->only_use_rotary_sliders);
                popup_linear_sensi_slider->setRange(100, 5000, 1);
                popup_linear_sensi_slider->setTextBoxStyle(Slider::NoTextBox, true, 70, 20);
                popup_linear_sensi_slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
                if (synth_data)
                {
                    popup_linear_sensi_slider->setValue(int(synth_data->sliders_linear_sensitivity),
                                                        dontSendNotification);
                }
                {
                    static bool fix_oss_port_issue = false;
                    jassert(fix_oss_port_issue);
                    fix_oss_port_issue = true;
                }
                /*
                slider_menu->addCustomItem (17,
                                            popup_linear_sensi_slider,
                                            150, 30,
                                            false );
                                            */
            }
            // ROTARY
            {
                slider_menu->addSeparator();
                slider_menu->addSectionHeader("ROTARY SLIDER HANDLING");
                slider_menu->addItem(12, TRANS("Velocity-Sensitive Mode (Shortcut: CTRL+drag)"),
                                     true, synth_data->is_rotary_sliders_velocity_mode);
                slider_menu->addItem(13, TRANS("Use Circular Dragging"), true,
                                     synth_data->sliders_in_rotary_mode and
                                         not synth_data->is_rotary_sliders_velocity_mode);
                // menu->addItem (14, TRANS ("Use left-right dragging"), true,
                // slider_->getSliderStyle() == Slider::SliderStyle::RotaryHorizontalDrag );
                // menu->addItem (15, TRANS ("Use up-down dragging"), true,
                // slider_->getSliderStyle() == Slider::SliderStyle::RotaryVerticalDrag );
                slider_menu->addItem(16, TRANS("Use Left-Right/Up-Down Dragging"), true,
                                     not synth_data->sliders_in_rotary_mode and
                                         not synth_data->is_rotary_sliders_velocity_mode);

                slider_menu->addSeparator();
                slider_menu->addSectionHeader("Rotary Slider Sensitivity");
                popup_rotary_sensi_slider->setRange(100, 2000, 1);
                popup_rotary_sensi_slider->setTextBoxStyle(Slider::NoTextBox, true, 70, 20);
                popup_rotary_sensi_slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
                popup_rotary_sensi_slider->setValue(int(synth_data->sliders_sensitivity),
                                                    dontSendNotification);
                {
                    static bool fix_oss_port_issue = false;
                    jassert(fix_oss_port_issue);
                    fix_oss_port_issue = true;
                }
                /*
                slider_menu->addCustomItem (18,
                                            popup_rotary_sensi_slider,
                                            150, 30,
                                            false );
                                            */
            }
            menu->addSubMenu("GLOBAL SLIDER HANDLING", *slider_menu, true);
        }

        // ANIMATIONS
        {
            menu->addSeparator();
            {
                animations_menu = new PopupMenu();
                animations_menu->addSectionHeader("ANIMATIONS");
                animations_menu->addItem(40, TRANS("Animate Envelopes (Buttons) (Shortcut CTRL+E)"),
                                         true, synth_data->animate_envs);
                animations_menu->addItem(41,
                                         TRANS("Animate Morph States (Sliders) (Shortcut CTRL+R)"),
                                         true, synth_data->animate_sliders);
                animations_menu->addItem(42, TRANS("Animate Arp (Buttons)"), true,
                                         synth_data->animate_arp);
                menu->addSubMenu("ANIMATIONS", *animations_menu, true);
            }
        }

        // MIDI
        {
            menu->addSeparator();
            menu->addSectionHeader("MIDI");
            menu->addItem(20, TRANS("Map MIDI Controller (Shortcut: CTRL+M)"), true, false);

            menu->addSectionHeader("SNAP TO MIDI INPUT");
            popup_midi_snap_slider->setRange(0, 1, 0.001);
            popup_midi_snap_slider->setTextBoxStyle(Slider::NoTextBox, true, 70, 20);
            popup_midi_snap_slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
            if (synth_data)
            {
                popup_midi_snap_slider->setValue(synth_data->midi_pickup_offset,
                                                 dontSendNotification);
            }
            {
                static bool fix_oss_port_issue = false;
                jassert(fix_oss_port_issue);
                fix_oss_port_issue = true;
            }
            /*
            menu->addCustomItem (21,
                                 popup_midi_snap_slider,
                                 150, 30,
                                 false ); */
        }

        // HELP
        {
            menu->addSeparator();
            {
                help_menu = new PopupMenu();
                help_menu->addSectionHeader("HELP");
                help_menu->addItem(27, TRANS("Show Wiring Diagram (Shortcut: CTRL+W"), true,
                                   synth_data->show_tooltips);
                help_menu->addItem(25, TRANS("Force Show ToolTip (Shortcut: CTRL+H)"), true, false);
                help_menu->addItem(26, TRANS("Show ToolTips Automatically (after 1000ms)"), true,
                                   synth_data->show_tooltips);
                menu->addSubMenu("HELP", *help_menu, true);
            }
        }

        // GLOBAL SETTINGS
        {
            menu->addSeparator();
            {
                settings_menu = new PopupMenu();
                settings_menu->addSectionHeader("GLOBAL SETTINGS");
                settings_menu->addSectionHeader(
                    "(Keep settings, colours & MIDI over multiple instances up to date)");
                settings_menu->addItem(
                    30, TRANS("Save Global Settings (auto saves on Monique shutdown)"), true,
                    false);
                settings_menu->addItem(31, TRANS("Load Global Settings"), true, false);

                menu->addSubMenu("GLOBAL SETTINGS", *settings_menu, true);
            }
        }

        // OPENGL
        {
#if JUCE_OPENGL
            menu->addSeparator();
            menu->addItem(32, "OpenGL Rendering Engine", true,
                          mainwindow ? mainwindow->openGLContext.isAttached() : false);
#endif
        }
        // MONO STEREO
        {
            menu->addSeparator();
            menu->addItem(33, "Stereo Processing", true, synth_data->is_stereo);
        }
    }
    /*
        bool isHorizontal() const noexcept;
    bool isVertical() const noexcept;
    void setScrollWheelEnabled (bool enabled);
    overwrite maximum
    overwrite minimum
    void setMouseDragSensitivity (int distanceForFullScaleDrag);
    int getMouseDragSensitivity() const noexcept;
    */
    return menu;
}

bool UiLookAndFeel::sliderMenuCallback(const int result, Slider *slider)
{
    if (slider != nullptr)
    {
        const Point<int> force_mouse_pos_point =
            Point<int>(slider->getScreenX() + slider->getWidth() * 0.5,
                       slider->getScreenY() + slider->getHeight() * 0.5);
        switch (result)
        {
        case 1:
            slider->setValue(slider->getProperties().getWithDefault(RETURN_VALUE_USER, 0),
                             sendNotificationSync);
            break;
        case 2:
            slider->setValue(slider->getProperties().getWithDefault(RETURN_VALUE_FACTORY, 0),
                             sendNotificationSync);
            break;
        case 3:
            slider->setValue(slider->getProperties().getWithDefault(RETURN_VALUE_PROGRAM, 0),
                             sendNotificationSync);
            break;
        case 4:
            slider->setValue(
                slider->getProperties().getWithDefault(RETURN_VALUE_UNDO, slider->getValue()),
                sendNotificationSync);
            break;
        case 5:
            slider->getProperties().set(RETURN_VALUE_USER, slider->getValue());
            break;
        case 6:
            is_global_user_return = true;
            is_global_factory_return = false;
            is_global_program_return = false;
            is_global_undo_return = false;
            break;
        case 7:
            is_global_factory_return = true;
            is_global_user_return = false;
            is_global_program_return = false;
            is_global_undo_return = false;
            break;
        case 8:
            is_global_program_return = true;
            is_global_user_return = false;
            is_global_factory_return = false;
            is_global_undo_return = false;
            break;
        case 9:
            is_global_undo_return = true;
            is_global_user_return = false;
            is_global_factory_return = false;
            is_global_program_return = false;
            break;
        case 10:
        {
            // synth_data->glide_motor_time =
        }
        break;
        case 11:
            synth_data->is_linear_sliders_velocity_mode ^= true;
            if (mainwindow)
            {
                mainwindow->global_slider_settings_changed(mainwindow);
            }
            break;
        case 12:
            synth_data->is_rotary_sliders_velocity_mode ^= true;
            if (mainwindow)
            {
                mainwindow->global_slider_settings_changed(mainwindow);
            }
            break;
        case 13:
            synth_data->sliders_in_rotary_mode = true;
            synth_data->is_rotary_sliders_velocity_mode = false;
            if (mainwindow)
            {
                mainwindow->global_slider_settings_changed(mainwindow);
            }
            break;
        case 14:
            synth_data->only_use_rotary_sliders ^= true;
            if (mainwindow)
            {
                mainwindow->global_slider_settings_changed(mainwindow);
            }
            break;

        case 16:
            synth_data->sliders_in_rotary_mode = false;
            synth_data->is_rotary_sliders_velocity_mode = false;
            if (mainwindow)
            {
                mainwindow->global_slider_settings_changed(mainwindow);
            }
            break;
        case 17:
            slider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
            break;
        case 18:
            slider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
            break;
            /*case 19:
                /est_parameter = slider->getValue();
                break;*/
        case 20:
            if (mainwindow)
            {
                Desktop::getInstance().setMousePosition(force_mouse_pos_point);
                mainwindow->show_info_popup(slider, nullptr, true);
            }
            break;
        case 25:
            if (mainwindow)
            {
                if (not force_tip)
                {
                    force_tip = new TooltipWindow(nullptr, 5);
                }
                Desktop::getInstance().setMousePosition(force_mouse_pos_point);
                {
                    static bool fix_oss_port_issue = false;
                    jassert(fix_oss_port_issue);
                    fix_oss_port_issue = true;
                }
                // force_tip->force_for_component( slider );
                force_tip->displayTip(force_mouse_pos_point, slider->getTooltip());
            }
            break;
        case 26:
            if (mainwindow)
            {
                synth_data->show_tooltips ^= true;
                mainwindow->update_tooltip_handling(false);
            }
            break;
        case 27:
            if (mainwindow)
            {
                mainwindow->show_overlay();
            }
            break;
        case 30:
            synth_data->save_settings();
            synth_data->save_midi();
            break;
        case 31:
            synth_data->load_settings();
            synth_data->read_midi();
            break;
        case 32:
            if (mainwindow)
            {
#if JUCE_OPENGL
                if (mainwindow->openGLContext.isAttached())
                {
                    mainwindow->openGLContext.detach();
                }
                else
                {
                    mainwindow->setOpenGLRenderingEngine();
                }
#endif
            }
            break;
        case 33:
            if (mainwindow)
            {
                synth_data->set_to_stereo(not synth_data->is_stereo);
            }
            break;
        case 40:
            synth_data->animate_envs ^= true;
            break;
        case 41:
            synth_data->animate_sliders ^= true;
            break;
        case 42:
            synth_data->animate_arp ^= true;
            break;
        default:
            break;
        }
    }

    return true;
}
bool UiLookAndFeel::sliderDoubleClicked(Slider *slider)
{
    if (is_global_user_return)
    {
        slider->setValue(float(slider->getProperties().getWithDefault(RETURN_VALUE_USER, 0)),
                         sendNotificationSync);
    }
    else if (is_global_factory_return)
    {
        slider->setValue(float(slider->getProperties().getWithDefault(RETURN_VALUE_FACTORY, 0)),
                         sendNotificationSync);
    }
    else if (is_global_program_return)
    {
        slider->setValue(float(slider->getProperties().getWithDefault(RETURN_VALUE_PROGRAM, 0)),
                         sendNotificationSync);
    }
    else
    {
        const float current_value = slider->getValue();
        slider->setValue(
            float(slider->getProperties().getWithDefault(RETURN_VALUE_UNDO, current_value)),
            sendNotificationSync);
        slider->getProperties().set(RETURN_VALUE_UNDO, current_value);
    }

    return true;
}
void UiLookAndFeel::sliderValueChanged(Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == popup_smooth_Slider)
    {
        if (synth_data)
        {
            synth_data->glide_motor_time = sliderThatWasMoved->getValue();
        }
        if (mainwindow)
        {
            mainwindow->global_slider_settings_changed(mainwindow);
        }
    }
    else if (sliderThatWasMoved == popup_linear_sensi_slider)
    {
        if (synth_data)
        {
            synth_data->sliders_linear_sensitivity = sliderThatWasMoved->getValue();
        }
        if (mainwindow)
        {
            mainwindow->global_slider_settings_changed(mainwindow);
        }
    }
    else if (sliderThatWasMoved == popup_rotary_sensi_slider)
    {
        if (synth_data)
        {
            synth_data->sliders_sensitivity = sliderThatWasMoved->getValue();
        }
        if (mainwindow)
        {
            mainwindow->global_slider_settings_changed(mainwindow);
        }
    }
    else if (sliderThatWasMoved == popup_midi_snap_slider)
    {
        synth_data->midi_pickup_offset = sliderThatWasMoved->getValue();
    }
}

//==============================================================================
Rectangle<int> UiLookAndFeel::getTooltipBounds(const String &tipText, Point<int> screenPos,
                                               Rectangle<int> parentArea)
{
    const TextLayout tl(
        LookAndFeelHelpers::layoutTooltipText(tipText, Colours::black, defaultFont));

    const int w = (int)(tl.getWidth() + 30.0f);
    const int h = (int)(tl.getHeight() + 20.0f);

    return Rectangle<int>(
               screenPos.x > parentArea.getCentreX() ? screenPos.x - (w + 12) : screenPos.x + 24,
               screenPos.y > parentArea.getCentreY() ? screenPos.y - (h + 6) : screenPos.y + 6, w,
               h)
        .constrainedWithin(parentArea);
}

void UiLookAndFeel::drawTooltip(Graphics &g, const String &text, int width, int height)
{
    g.fillAll(findColour(TooltipWindow::backgroundColourId));

    //#if ! JUCE_MAC // The mac windows already have a non-optional 1 pix outline, so don't double
    // it here..
    g.setColour(findColour(TooltipWindow::outlineColourId));
    g.drawRect(0, 0, width, height, 1);
    //#endif

    Rectangle<float> rect = Rectangle<float>((float)width, (float)height);
    TextLayout tl = LookAndFeelHelpers::layoutTooltipText(
        text, findColour(TooltipWindow::textColourId), defaultFont);
    int center_rest = rect.getWidth() - tl.getWidth();
    rect.setX(center_rest / 2);
    tl.draw(g, rect);
}

//==============================================================================
void UiLookAndFeel::drawCornerResizer(Graphics &g, int w, int h, bool /*isMouseOver*/,
                                      bool /*isMouseDragging*/)
{
    const float lineThickness = jmin(w, h) * 0.075f;

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        g.setColour(colours.get_theme(BG_THEME).area_font_colour);
        g.drawLine(w * i + lineThickness, h + 1.0f, w + 1.0f, h * i + lineThickness, lineThickness);
    }
}

//==============================================================================
void UiLookAndFeel::fillResizableWindowBackground(Graphics &g, int /*w*/, int /*h*/,
                                                  const BorderSize<int> & /*border*/,
                                                  ResizableWindow &window)
{
}

void UiLookAndFeel::drawResizableWindowBorder(Graphics &g, int /*w*/, int /*h*/,
                                              const BorderSize<int> & /*border*/, ResizableWindow &)
{
    // g.fillAll(Colours::black);
}

void UiLookAndFeel::drawDocumentWindowTitleBar(DocumentWindow &window, Graphics &g, int w, int h,
                                               int titleSpaceX, int titleSpaceW, const Image *icon,
                                               bool drawTitleTextOnLeft)
{
    const bool isActive = window.isActiveWindow();

    g.fillAll(window.getBackgroundColour().contrasting(isActive ? 0.0f : 0.25f));

    Font font = defaultFont.withHeight(h * 0.65f);
    g.setFont(font);

    int textW = font.getStringWidth(window.getName());
    int iconW = 0;
    int iconH = 0;

    if (icon != nullptr)
    {
        iconH = (int)font.getHeight();
        iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
    }

    textW = jmin(titleSpaceW, textW + iconW);
    int textX = drawTitleTextOnLeft ? titleSpaceX : jmax(titleSpaceX, (w - textW) / 2);

    if (textX + textW > titleSpaceX + titleSpaceW)
        textX = titleSpaceX + titleSpaceW - textW;

    if (icon != nullptr)
    {
        g.setOpacity(isActive ? 1.0f : 0.6f);
        g.drawImageWithin(*icon, textX, (h - iconH) / 2, iconW, iconH, RectanglePlacement::centred,
                          false);
        textX += iconW;
        textW -= iconW;
    }

    if (window.isColourSpecified(DocumentWindow::textColourId) ||
        isColourSpecified(DocumentWindow::textColourId))
        g.setColour(window.findColour(DocumentWindow::textColourId));
    else
        g.setColour(window.getBackgroundColour().contrasting(isActive ? 0.7f : 0.4f));

    g.drawText(window.getName(), textX, 0, textW, h, Justification::centredLeft, true);
}

//==============================================================================
class UiLookAndFeel::GlassWindowButton : public Button
{
  public:
    GlassWindowButton(const String &name, Colour col, const Path &normalShape_,
                      const Path &toggledShape_) noexcept
        : Button(name), colour(col), normalShape(normalShape_), toggledShape(toggledShape_)
    {
    }

    //==============================================================================
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown) override
    {
        float alpha = isMouseOverButton ? (isButtonDown ? 1.0f : 0.8f) : 0.55f;

        if (!isEnabled())
            alpha *= 0.5f;

        float x = 0, y = 0, diam;

        if (getWidth() < getHeight())
        {
            diam = (float)getWidth() - 4;
            y = (getHeight() - getWidth()) * 0.5f;
        }
        else
        {
            diam = (float)getHeight() - 4;
            y = (getWidth() - getHeight()) * 0.5f;
        }

        x += diam * 0.05f + 2;
        y += diam * 0.05f + 2;
        diam *= 0.9f;

        g.setColour(Colour::greyLevel(0.6f).withAlpha(
            alpha)); // , 0, y + diam, Colour::greyLevel (0.6f).withAlpha (alpha), 0, y, false
        g.fillEllipse(x, y, diam, diam);

        x += 2.0f;
        y += 2.0f;
        diam -= 4.0f;

        UiLookAndFeel::drawGlassSphere(g, x, y, diam, colour, 1.0f);

        Path &p = getToggleState() ? toggledShape : normalShape;

        const AffineTransform t(p.getTransformToScaleToFit(x + diam * 0.3f, y + diam * 0.3f,
                                                           diam * 0.4f, diam * 0.4f, true));

        g.setColour(Colours::black.withAlpha(alpha * 0.6f));
        g.fillPath(p, t);
    }

  private:
    Colour colour;
    Path normalShape, toggledShape;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlassWindowButton)
};

Button *UiLookAndFeel::createDocumentWindowButton(int buttonType)
{
    Path shape;
    const float crossThickness = 0.25f;

    if (buttonType == DocumentWindow::closeButton)
    {
        shape.addLineSegment(Line<float>(0.0f, 0.0f, 1.0f, 1.0f), crossThickness * 1.4f);
        shape.addLineSegment(Line<float>(1.0f, 0.0f, 0.0f, 1.0f), crossThickness * 1.4f);

        return new GlassWindowButton("close", Colour(0xff991100), shape, shape);
    }

    if (buttonType == DocumentWindow::minimiseButton)
    {
        shape.addLineSegment(Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        return new GlassWindowButton("minimise", Colour(0xffdfce89), shape, shape);
    }

    if (buttonType == DocumentWindow::maximiseButton)
    {
        shape.addLineSegment(Line<float>(0.5f, 0.0f, 0.5f, 1.0f), crossThickness);
        shape.addLineSegment(Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        Path fullscreenShape;
        fullscreenShape.startNewSubPath(45.0f, 100.0f);
        fullscreenShape.lineTo(0.0f, 100.0f);
        fullscreenShape.lineTo(0.0f, 0.0f);
        fullscreenShape.lineTo(100.0f, 0.0f);
        fullscreenShape.lineTo(100.0f, 45.0f);
        fullscreenShape.addRectangle(45.0f, 45.0f, 100.0f, 100.0f);
        PathStrokeType(30.0f).createStrokedPath(fullscreenShape, fullscreenShape);

        return new GlassWindowButton("maximise", Colour(0xffdfce89), shape, fullscreenShape);
    }

    jassertfalse;
    return nullptr;
}

void UiLookAndFeel::positionDocumentWindowButtons(DocumentWindow &, int titleBarX, int titleBarY,
                                                  int titleBarW, int titleBarH,
                                                  Button *minimiseButton, Button *maximiseButton,
                                                  Button *closeButton,
                                                  bool positionTitleBarButtonsOnLeft)
{
    const int buttonW = titleBarH - titleBarH / 8;

    int x = positionTitleBarButtonsOnLeft ? titleBarX + 4
                                          : titleBarX + titleBarW - buttonW - buttonW / 4;

    if (closeButton != nullptr)
    {
        closeButton->setBounds(x, titleBarY, buttonW, titleBarH);
        x += positionTitleBarButtonsOnLeft ? buttonW : -(buttonW + buttonW / 4);
    }

    if (positionTitleBarButtonsOnLeft)
        std::swap(minimiseButton, maximiseButton);

    if (maximiseButton != nullptr)
    {
        maximiseButton->setBounds(x, titleBarY, buttonW, titleBarH);
        x += positionTitleBarButtonsOnLeft ? buttonW : -buttonW;
    }

    if (minimiseButton != nullptr)
        minimiseButton->setBounds(x, titleBarY, buttonW, titleBarH);
}

//==============================================================================
void UiLookAndFeel::drawShinyButtonShape(Graphics &g, float x, float y, float w, float h,
                                         float maxCornerSize, const Colour &baseColour,
                                         const float strokeWidth, const bool flatOnLeft,
                                         const bool flatOnRight, const bool flatOnTop,
                                         const bool flatOnBottom) noexcept
{
    if (w <= strokeWidth * 1.1f || h <= strokeWidth * 1.1f)
        return;

    const float cs = jmin(maxCornerSize, w * 0.5f, h * 0.5f);

    Path outline;
    outline.addRoundedRectangle(x, y, w, h, cs, cs, !(flatOnLeft || flatOnTop),
                                !(flatOnRight || flatOnTop), !(flatOnLeft || flatOnBottom),
                                !(flatOnRight || flatOnBottom));

    ColourGradient cg(baseColour, 0.0f, y, baseColour.overlaidWith(Colour(0x070000ff)), 0.0f, y + h,
                      false);

    cg.addColour(0.5, baseColour.overlaidWith(Colour(0x33ffffff)));
    cg.addColour(0.51, baseColour.overlaidWith(Colour(0x110000ff)));

    g.setGradientFill(cg);
    g.fillPath(outline);

    g.setColour(Colour(0x80000000));
    g.strokePath(outline, PathStrokeType(strokeWidth));
}

//==============================================================================
void UiLookAndFeel::drawGlassSphere(Graphics &g, const float x, const float y, const float diameter,
                                    const Colour &colour, const float outlineThickness) noexcept
{
    if (diameter <= outlineThickness)
        return;

    Path p;
    p.addEllipse(x, y, diameter, diameter);

    {
        g.setColour(colour);
        g.fillPath(p);
    }

    // g.setGradientFill (ColourGradient (Colours::white, 0, y + diameter * 0.06f,
    // Colours::transparentWhite, 0, y + diameter * 0.3f, false));
    // g.fillEllipse (x + diameter * 0.2f, y + diameter * 0.05f, diameter * 0.6f, diameter * 0.4f);

    ColourGradient cg(Colours::transparentBlack, x + diameter * 0.5f, y + diameter * 0.5f,
                      Colours::black.withAlpha(0.5f * outlineThickness * colour.getFloatAlpha()), x,
                      y + diameter * 0.5f, true);

    // cg.addColour (0.7, Colours::transparentBlack);
    // cg.addColour (0.8, Colours::black.withAlpha (0.1f * outlineThickness));

    // g.setGradientFill (cg);
    // g.fillPath (p);

    g.setColour(Colours::black.withAlpha(0.5f * colour.getFloatAlpha()));
    g.drawEllipse(x, y, diameter, diameter, outlineThickness);
}

//==============================================================================
void UiLookAndFeel::drawGlassPointer(Graphics &g, const float x, const float y,
                                     const float diameter, const Colour &colour,
                                     const float outlineThickness, const int direction) noexcept
{
    if (diameter <= outlineThickness)
        return;

    Path p;
    p.startNewSubPath(x + diameter * 0.5f, y);
    p.lineTo(x + diameter, y + diameter * 0.6f);
    p.lineTo(x + diameter, y + diameter);
    p.lineTo(x, y + diameter);
    p.lineTo(x, y + diameter * 0.6f);
    p.closeSubPath();

    p.applyTransform(AffineTransform::rotation(direction * (juce::MathConstants<float>::pi * 0.5f),
                                               x + diameter * 0.5f, y + diameter * 0.5f));

    {
        ColourGradient cg(Colours::white.overlaidWith(colour.withMultipliedAlpha(0.3f)), 0, y,
                          Colours::white.overlaidWith(colour.withMultipliedAlpha(0.3f)), 0,
                          y + diameter, false);

        cg.addColour(0.4, Colours::white.overlaidWith(colour));

        g.setGradientFill(cg);
        g.fillPath(p);
    }

    ColourGradient cg(Colours::transparentBlack, x + diameter * 0.5f, y + diameter * 0.5f,
                      Colours::black.withAlpha(0.5f * outlineThickness * colour.getFloatAlpha()),
                      x - diameter * 0.2f, y + diameter * 0.5f, true);

    cg.addColour(0.5, Colours::transparentBlack);
    cg.addColour(0.7, Colours::black.withAlpha(0.07f * outlineThickness));

    g.setGradientFill(cg);
    g.fillPath(p);

    g.setColour(Colours::black.withAlpha(0.5f * colour.getFloatAlpha()));
    g.strokePath(p, PathStrokeType(outlineThickness));
}

//==============================================================================
void UiLookAndFeel::drawGlassLozenge(Graphics &g, const float x, const float y, const float width,
                                     const float height, const Colour &colour,
                                     const float outlineThickness, const float cornerSize,
                                     const bool flatOnLeft, const bool flatOnRight,
                                     const bool flatOnTop, const bool flatOnBottom) noexcept
{
    Path outline;
    outline.addRoundedRectangle(x, y, width, height, 2);

    {
        g.setColour(colour);
        g.fillPath(outline);
    }
}
