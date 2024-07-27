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

#include "monique_ui_LookAndFeel.h"

#include "monique_ui_MainWindow.h"
#include "core/monique_core_Datastructures.h"

#include <BinaryData.h>
#include <memory>

#define CAN_OPAQUE true
//==============================================================================
//==============================================================================
//==============================================================================
namespace LookAndFeelHelpers
{
static juce::Colour createBaseColour(juce::Colour buttonColour, bool hasKeyboardFocus,
                                     bool isMouseOverButton, bool isButtonDown) noexcept
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const juce::Colour baseColour(buttonColour.withMultipliedSaturation(sat));

    if (isButtonDown)
        return baseColour.contrasting(0.2f);
    if (isMouseOverButton)
        return baseColour.contrasting(0.1f);

    return baseColour;
}

static juce::TextLayout layoutTooltipText(const juce::String &text, juce::Colour colour,
                                          juce::Font &font_) noexcept
{
    const int maxToolTipWidth = 530;

    juce::AttributedString s;
    s.setJustification(juce::Justification::centredLeft);
    s.append(
        text +
            juce::String("\n\n___________________________________________________________________"
                         "______\n\nNERVES ARE ON THE EDGE?\n-----------------------\nTool tips "
                         "you can disable in the setup."),
        juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::SourceCodeProMedium_otf,
                                                           BinaryData::SourceCodeProMedium_otfSize))
            .withHeight(15.0f),
        // font_,
        colour);

    juce::TextLayout tl;
    tl.createLayoutWithBalancedLineLengths(s, (float)maxToolTipWidth);
    return tl;
}
} // namespace LookAndFeelHelpers

//==============================================================================
//==============================================================================
//==============================================================================
ComponentColours::ComponentColours() noexcept : midi_learn(juce::Colours::red) { init_themes(); }
ComponentColours::~ComponentColours() noexcept {}

//==============================================================================
void ComponentColours::read_from(juce::XmlElement *xml_) noexcept
{
    juce::XmlElement *xml = xml_->getChildByName("COLOURS");
    if (xml)
    {
        for (int i = 0; i != themes.size(); ++i)
        {
            SectionTheme *theme = themes.getUnchecked(i);

            auto pfx = juce::String("c_") + juce::String(i);

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

            theme->area_colour = juce::Colour::fromString(
                readNewOld(pfx + "_area_colour", theme->area_colour.toString()));
            theme->area_font_colour = juce::Colour::fromString(
                readNewOld(pfx + "_area_font_colour", theme->area_font_colour.toString()));
            theme->value_slider_track_colour = juce::Colour::fromString(readNewOld(
                pfx + "_value_slider_track_colour", theme->value_slider_track_colour.toString()));
            theme->value_2_slider_track_colour =
                juce::Colour::fromString(readNewOld(pfx + "_value_2_slider_track_colour",
                                                    theme->value_2_slider_track_colour.toString()));
            theme->mod_slider_track_colour = juce::Colour::fromString(readNewOld(
                pfx + "_mod_slider_track_colour", theme->mod_slider_track_colour.toString()));
            theme->disabled_track_colour = juce::Colour::fromString(readNewOld(
                pfx + "_disabled_track_colour", theme->disabled_track_colour.toString()));
            theme->slider_bg_colour = juce::Colour::fromString(
                readNewOld(pfx + "_slider_bg_colour", theme->slider_bg_colour.toString()));
            theme->button_on_font_colour = juce::Colour::fromString(readNewOld(
                pfx + "_button_on_font_colour", theme->button_on_font_colour.toString()));
            theme->button_on_colour = juce::Colour::fromString(
                readNewOld(pfx + "_button_on_colour", theme->button_on_colour.toString()));
            theme->button_off_font_colour = juce::Colour::fromString(readNewOld(
                pfx + "_button_off_font_colour", theme->button_off_font_colour.toString()));
            theme->button_off_colour = juce::Colour::fromString(
                readNewOld(pfx + "_button_off_colour", theme->button_off_colour.toString()));

            theme->oszi_1 =
                juce::Colour::fromString(readNewOld(pfx + "_oszi_1", theme->oszi_1.toString()));
            theme->oszi_2 =
                juce::Colour::fromString(readNewOld(pfx + "_oszi_2", theme->oszi_2.toString()));
            theme->oszi_3 =
                juce::Colour::fromString(readNewOld(pfx + "_oszi_3", theme->oszi_3.toString()));
        }

        midi_learn = juce::Colour::fromString(
            xml->getStringAttribute("ml_col", juce::Colours::red.toString()));
    }
}
void ComponentColours::save_to(juce::XmlElement *xml_) noexcept
{
    juce::XmlElement *xml = xml_->createNewChildElement("COLOURS");
    if (xml)
    {
        for (int i = 0; i != themes.size(); ++i)
        {
            SectionTheme *theme = themes.getUnchecked(i);

            auto pfx = juce::String("c_") + juce::String(i);

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

      show_values_always(false, generate_param_name("LF", 1, "show_values_always"),
                         generate_short_human_name("LF", "show_values_always")),
      popup_smooth_Slider(new juce::Slider("")), popup_linear_sensi_slider(new juce::Slider("")),
      popup_rotary_sensi_slider(new juce::Slider("")),

      popup_midi_snap_slider(new juce::Slider(""))
{
#ifdef JUCE_DEBUG
    std::cout << "MONIQUE: init style" << std::endl;
#endif

    popup_smooth_Slider->addListener(this);
    popup_rotary_sensi_slider->addListener(this);
    popup_linear_sensi_slider->addListener(this);
    popup_midi_snap_slider->addListener(this);

    // initialise the standard set of colours..
    const std::uint32_t textButtonColour = 0xffbbbbff;
    const std::uint32_t textHighlightColour = 0x401111ee;
    const std::uint32_t standardOutlineColour = 0xb2808080;

    static const std::uint32_t standardColours[] = {
        juce::TextButton::buttonColourId,
        0xff000000,
        juce::TextButton::buttonOnColourId,
        0xff4444ff,
        juce::TextButton::textColourOnId,
        0xff000000,
        juce::TextButton::textColourOffId,
        0xff000000,

        juce::ToggleButton::textColourId,
        0xff000000,

        juce::TextEditor::backgroundColourId,
        0xffffffff,
        juce::TextEditor::textColourId,
        colours.get_theme(COLOUR_THEMES::BG_THEME).area_font_colour.getARGB(),
        juce::TextEditor::highlightColourId,
        0x66ff0000,
        juce::TextEditor::highlightedTextColourId,
        0xffffffff,
        juce::TextEditor::outlineColourId,
        0x00000000,
        juce::TextEditor::focusedOutlineColourId,
        textButtonColour,
        juce::TextEditor::shadowColourId,
        0x38000000,

        juce::CaretComponent::caretColourId,
        0xff000000,

        juce::Label::backgroundColourId,
        0x00000000,
        juce::Label::textColourId,
        0xffff3b00,
        juce::Label::outlineColourId,
        0x00000000,

        juce::ScrollBar::backgroundColourId,
        0x00000000,
        juce::ScrollBar::thumbColourId,
        0xffffffff,

        juce::TreeView::linesColourId,
        0x4c000000,
        juce::TreeView::backgroundColourId,
        0x00000000,
        juce::TreeView::dragAndDropIndicatorColourId,
        0x80ff0000,
        juce::TreeView::selectedItemBackgroundColourId,
        0x00000000,

        juce::PopupMenu::backgroundColourId,
        0xffffffff,
        juce::PopupMenu::textColourId,
        0xff000000,
        juce::PopupMenu::headerTextColourId,
        0xff000000,
        juce::PopupMenu::highlightedTextColourId,
        0xffffffff,
        juce::PopupMenu::highlightedBackgroundColourId,
        0x991111aa,

        juce::ComboBox::buttonColourId,
        0xffbbbbff,
        juce::ComboBox::outlineColourId,
        0xffff0000,
        juce::ComboBox::textColourId,
        0xffff0000,
        juce::ComboBox::backgroundColourId,
        0xff000000,
        juce::ComboBox::arrowColourId,
        0x99000000,

        juce::PropertyComponent::backgroundColourId,
        0x66ffffff,
        juce::PropertyComponent::labelTextColourId,
        0xffff3b00,

        juce::TextPropertyComponent::backgroundColourId,
        0xffffffff,
        juce::TextPropertyComponent::textColourId,
        0xffffffff,
        juce::TextPropertyComponent::outlineColourId,
        standardOutlineColour,

        juce::ListBox::backgroundColourId,
        0xffffffff,
        juce::ListBox::outlineColourId,
        standardOutlineColour,
        juce::ListBox::textColourId,
        0xff000000,

        juce::Slider::backgroundColourId,
        0x00000000,
        juce::Slider::thumbColourId,
        textButtonColour,
        juce::Slider::trackColourId,
        0x7fffffff,
        juce::Slider::rotarySliderFillColourId,
        0x00000000,
        juce::Slider::rotarySliderOutlineColourId,
        0x66000000,
        juce::Slider::textBoxTextColourId,
        juce::Colour(juce::Colours::yellowgreen).getARGB(),
        juce::Slider::textBoxBackgroundColourId,
        0xff000000,
        juce::Slider::textBoxHighlightColourId,
        textHighlightColour,
        juce::Slider::textBoxOutlineColourId,
        0xffff3b00,

        juce::ResizableWindow::backgroundColourId,
        0xff777777,
        // DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)

        juce::AlertWindow::backgroundColourId,
        0xff161616,
        juce::AlertWindow::textColourId,
        0xffffffff,
        juce::AlertWindow::outlineColourId,
        0xffffffff,

        juce::ProgressBar::backgroundColourId,
        0xffeeeeee,
        juce::ProgressBar::foregroundColourId,
        0xffaaaaee,

        juce::TooltipWindow::backgroundColourId,
        0xff000000,
        juce::TooltipWindow::textColourId,
        0xffffff99,
        juce::TooltipWindow::outlineColourId,
        0xffffff99,

        juce::TabbedComponent::backgroundColourId,
        0x00000000,
        juce::TabbedComponent::outlineColourId,
        0xff777777,
        juce::TabbedButtonBar::tabOutlineColourId,
        0x80000000,
        juce::TabbedButtonBar::frontOutlineColourId,
        0x90000000,

        juce::Toolbar::backgroundColourId,
        0xfff6f8f9,
        juce::Toolbar::separatorColourId,
        0x4c000000,
        juce::Toolbar::buttonMouseOverBackgroundColourId,
        0x4c0000ff,
        juce::Toolbar::buttonMouseDownBackgroundColourId,
        0x800000ff,
        juce::Toolbar::labelTextColourId,
        0xffff3b00,
        juce::Toolbar::editingModeOutlineColourId,
        0xffff0000,

        juce::DrawableButton::textColourId,
        0xff000000,
        juce::DrawableButton::textColourOnId,
        0xff000000,
        juce::DrawableButton::backgroundColourId,
        0x00000000,
        juce::DrawableButton::backgroundOnColourId,
        0xaabbbbff,

        juce::HyperlinkButton::textColourId,
        0xcc1111ee,

        juce::GroupComponent::outlineColourId,
        0x66000000,
        juce::GroupComponent::textColourId,
        0xff000000,

        juce::BubbleComponent::backgroundColourId,
        0xeeeeeebb,
        juce::BubbleComponent::outlineColourId,
        0x77000000,

        juce::DirectoryContentsDisplayComponent::highlightColourId,
        textHighlightColour,
        juce::DirectoryContentsDisplayComponent::textColourId,
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

        juce::FileSearchPathListComponent::backgroundColourId,
        0xffffffff,

        juce::FileChooserDialogBox::titleTextColourId,
        0xffff3b00,
    };

    show_values_always = false;
    midi_learn_comp = nullptr;

    for (int i = 0; i < juce::numElementsInArray(standardColours); i += 2)
        setColour((int)standardColours[i], juce::Colour((std::uint32_t)standardColours[i + 1]));

    defaultFont = juce::Font(juce::Typeface::createSystemTypefaceFor(
                                 BinaryData::LatoSemibold_ttf, BinaryData::LatoSemibold_ttfSize))
                      .withHeight(15.0f);
    auto xml = juce::XmlDocument::parse(BinaryData::DARK_mcol);
    colours.read_from(xml.get());
    // defaultFont =
    // Font(Typeface::createSystemTypefaceFor(BinaryData::Tahoma_ttf,BinaryData::Tahoma_ttfSize));
    // defaultFont =
    // Font(Typeface::createSystemTypefaceFor(BinaryData::Segoe,BinaryData::SegoeSize));
}
UiLookAndFeel::~UiLookAndFeel() noexcept {}

//==============================================================================
void UiLookAndFeel::drawButtonBackground(juce::Graphics &g, juce::Button &button,
                                         const juce::Colour &backgroundColour,
                                         bool isMouseOverButton, bool isButtonDown)
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

    const bool is_midi_learn_mode = static_cast<juce::Component *>(&button) == midi_learn_comp;

    // isMouseOverButton
    juce::Colour color_1;
    if (is_midi_learn_mode)
    {
        color_1 =
            isButtonDown ? colours.midi_learn.darker(0.4f) : colours.midi_learn.brighter(0.25f);
    }
    else if (override_theme_colour)
    {
        color_1 = button.findColour(juce::TextButton::buttonColourId);
    }
    else if (amp == COLOR_REPLACEMENTS::FORCE_BIT_RED)
    {
        color_1 = theme.button_off_colour.interpolatedWith(juce::Colours::red, 0.2f);
    }
    else if (amp == COLOR_REPLACEMENTS::FORCE_RED)
    {
        color_1 = theme.button_on_colour.interpolatedWith(juce::Colours::red, 0.9f);
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
    if (is_forced_off)
            {
                col = theme->button_off_colour.interpolatedWith(Colours::red,0.2f);
            }
            else if (is_forced_on)
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
    else if (!button.isEnabled())
    {
        color_1 =  Colour(backgroundColour).withAlpha(0.5f);
    }
    else
    {
        color_1 =  isButtonDown ? Colour(backgroundColour).darker (0.4f) : Colour(backgroundColour);
    }
    */

    {
        g.setColour(is_enabled ? color_1
                               : color_1.interpolatedWith(juce::Colour(0x55333333), 0.3f));

        const bool flatOnLeft = button.isConnectedOnLeft();
        const float flatOnRight = button.isConnectedOnRight();
        const float flatOnTop = button.isConnectedOnTop();
        const float flatOnBottom = button.isConnectedOnBottom();
        juce::Path highlight;
        highlight.addRoundedRectangle(1, 1, button.getWidth() - 2, button.getHeight() - 2, 4, 4,
                                      !(flatOnLeft || flatOnTop), !(flatOnRight || flatOnTop),
                                      !(flatOnLeft || flatOnBottom),
                                      !(flatOnRight || flatOnBottom));

        g.fillPath(highlight);
    }
}

juce::Font UiLookAndFeel::getTextButtonFont(juce::TextButton &button, int buttonHeigh)
{
    return defaultFont; // button.getFont();
}
#define FONT_SCALE 0.55f
#define IDENT_SCALE (1.0f - FONT_SCALE) * 0.5
void UiLookAndFeel::drawButtonText(juce::Graphics &g, juce::TextButton &button,
                                   bool /*isMouseOverButton*/, bool /*isButtonDown*/)
{
    const float height = button.getHeight();
    const float width = button.getWidth();
    const float fontHeight = FONT_SCALE * height;
    const float yIndent = IDENT_SCALE * height;
    const float cornerSize = juce::jmin(height, width) * 0.2;
    const float leftIndent =
        juce::jmin(fontHeight, 1.0f + cornerSize / (button.isConnectedOnLeft() ? 4 : 4));
    const float rightIndent =
        juce::jmin(fontHeight, 1.0f + cornerSize / (button.isConnectedOnRight() ? 4 : 4));

    const SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(button.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    const bool override_theme_colour =
        button.getProperties().getWithDefault(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, false);
    const bool is_enabled = button.isEnabled();
    const float amp(button.getProperties().getWithDefault(VAR_INDEX_BUTTON_AMP, 0.0f));

    const bool is_toggled = amp != 0;
    juce::Colour color_1;
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
                      ? button.findColour(juce::TextButton::buttonColourId).contrasting(1)
                  : is_toggled ? theme.button_on_font_colour
                               : theme.button_off_font_colour;
    }
    g.setFont(defaultFont.withHeight(fontHeight));
    g.setColour(is_enabled ? color_1 : color_1.interpolatedWith(juce::Colour(0x55333333), 0.3f));
    g.drawText(
        button.getButtonText(),
        juce::Rectangle<float>(leftIndent, yIndent, (width - leftIndent - rightIndent), fontHeight),
        juce::Justification::centred, false);
}

void UiLookAndFeel::drawTickBox(juce::Graphics &g, juce::Component &component, float x, float y,
                                float w, float h, const bool ticked, const bool isEnabled,
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

void UiLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                                     bool isMouseOverButton, bool isButtonDown)
{
    // if( CAN_OPAQUE && button.isOpaque() )
    //    g.fillAll (colours.bg);

    float prop_h = 1.f / 25.f * button.getHeight();
    float top = prop_h * 4.f;
    float rect_size = button.getHeight() - top * 2;
    drawTickBox(g, button, (button.getWidth() - rect_size) / 2, top, rect_size, rect_size,
                button.getToggleState(), button.isEnabled(), isMouseOverButton, isButtonDown);
}

//==============================================================================
juce::AlertWindow *UiLookAndFeel::createAlertWindow(
    const juce::String &title, const juce::String &message, const juce::String &button1,
    const juce::String &button2, const juce::String &button3,
    juce::AlertWindow::AlertIconType iconType, int numButtons, juce::Component *associatedComponent)
{
    juce::AlertWindow *aw = new juce::AlertWindow(title, message, iconType, associatedComponent);

    if (numButtons == 1)
    {
        aw->addButton(button1, 0, juce::KeyPress(juce::KeyPress::escapeKey),
                      juce::KeyPress(juce::KeyPress::returnKey));
    }
    else
    {
        const juce::KeyPress button1ShortCut((int)juce::CharacterFunctions::toLowerCase(button1[0]),
                                             0, 0);
        juce::KeyPress button2ShortCut((int)juce::CharacterFunctions::toLowerCase(button2[0]), 0,
                                       0);
        if (button1ShortCut == button2ShortCut)
            button2ShortCut = juce::KeyPress();

        if (numButtons == 2)
        {
            aw->addButton(button1, 1, juce::KeyPress(juce::KeyPress::returnKey), button1ShortCut);
            aw->addButton(button2, 0, juce::KeyPress(juce::KeyPress::escapeKey), button2ShortCut);
        }
        else if (numButtons == 3)
        {
            aw->addButton(button1, 1, button1ShortCut);
            aw->addButton(button2, 2, button2ShortCut);
            aw->addButton(button3, 0, juce::KeyPress(juce::KeyPress::escapeKey));
        }
    }
    for (int i = 0; i != aw->getNumChildComponents(); ++i)
    {
        if (juce::Button *button = dynamic_cast<juce::Button *>(aw->getChildComponent(i)))
        {
            button->setColour(juce::TextButton::buttonColourId, juce::Colours::white);
        }
    }

    return aw;
}

void UiLookAndFeel::drawAlertBox(juce::Graphics &g, juce::AlertWindow &alert,
                                 const juce::Rectangle<int> &textArea, juce::TextLayout &textLayout)
{
    // g.fillAll (alert.findColour (AlertWindow::backgroundColourId));

    g.fillRoundedRectangle(0, 0, alert.getWidth(), alert.getHeight(), 10);

    int iconSpaceUsed = 0;

    const int iconWidth = 50;
    int iconSize = juce::jmin(iconWidth + 50, alert.getHeight() + 20);

    if (alert.containsAnyExtraComponents() || alert.getNumButtons() > 2)
        iconSize = juce::jmin(iconSize, textArea.getHeight() + 50);

    const juce::Rectangle<int> iconRect(iconSize / -10, iconSize / -10, iconSize, iconSize);

    if (alert.getAlertType() != juce::AlertWindow::NoIcon)
    {
        juce::Path icon;
        std::uint32_t colour;
        char character;

        if (alert.getAlertType() == juce::AlertWindow::WarningIcon)
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
            colour = alert.getAlertType() == juce::AlertWindow::InfoIcon
                         ? (std::uint32_t)0x605555ff
                         : (std::uint32_t)0x40b69900;
            character = alert.getAlertType() == juce::AlertWindow::InfoIcon ? 'i' : '?';

            icon.addEllipse((float)iconRect.getX(), (float)iconRect.getY(),
                            (float)iconRect.getWidth(), (float)iconRect.getHeight());
        }

        juce::GlyphArrangement ga;
        ga.addFittedText(juce::Font(iconRect.getHeight() * 0.9f, juce::Font::bold),
                         juce::String::charToString((juce::juce_wchar)(std::uint8_t)character),
                         (float)iconRect.getX(), (float)iconRect.getY(), (float)iconRect.getWidth(),
                         (float)iconRect.getHeight(), juce::Justification::centred, false);
        ga.createPath(icon);

        icon.setUsingNonZeroWinding(false);
        g.setColour(juce::Colour(colour));
        g.fillPath(icon);

        iconSpaceUsed = iconWidth;
    }

    g.setColour(alert.findColour(juce::AlertWindow::textColourId));

    textLayout.draw(g,
                    juce::Rectangle<int>(textArea.getX() + iconSpaceUsed, textArea.getY(),
                                         textArea.getWidth() - iconSpaceUsed, textArea.getHeight())
                        .toFloat());

    g.setColour(alert.findColour(juce::AlertWindow::outlineColourId));
    g.drawRect(0, 0, alert.getWidth(), alert.getHeight(), 2);
}

int UiLookAndFeel::getAlertBoxWindowFlags()
{
    return juce::ComponentPeer::windowAppearsOnTaskbar | juce::ComponentPeer::windowHasDropShadow;
}

int UiLookAndFeel::getAlertWindowButtonHeight()
{
    return int(
        1.f / 900 *
        juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight() * 30);
}

juce::Font UiLookAndFeel::getAlertWindowMessageFont()
{
    return defaultFont.withHeight(
        1.f / 900 *
        juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight() * 20);
}

juce::Font UiLookAndFeel::getAlertWindowFont()
{
    return defaultFont.withHeight((
        1.f / 900 *
        juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight() * 17));
}

//==============================================================================
juce::Font UiLookAndFeel::getPopupMenuFont() { return defaultFont; }

void UiLookAndFeel::getIdealPopupMenuItemSize(const juce::String &text, const bool isSeparator,
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
        juce::Font font(getPopupMenuFont());

        if (standardMenuItemHeight > 0 && font.getHeight() > standardMenuItemHeight / 1.3f)
            font.setHeight(standardMenuItemHeight / 1.3f);

        idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight
                                                 : juce::roundToInt(font.getHeight() * 1.3f);
        idealWidth = font.getStringWidth(text) + idealHeight * 2;
    }
}

void UiLookAndFeel::drawPopupMenuBackground(juce::Graphics &g, int width, int height)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    const juce::Colour background(theme.button_off_colour);

    g.fillAll(background);
    g.setColour(background.overlaidWith(theme.area_font_colour.withAlpha(0.07f)));

    for (int i = 0; i < height; i += 3)
        g.fillRect(0, i, width, 1);

#if !JUCE_MAC
    g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.6f));
    g.drawRect(0, 0, width, height);
#endif
}

void UiLookAndFeel::drawPopupMenuUpDownArrow(juce::Graphics &g, int width, int height,
                                             bool isScrollUpArrow)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    const juce::Colour background(theme.button_on_colour);

    g.setGradientFill(juce::ColourGradient(background, 0.0f, height * 0.5f,
                                           background.withAlpha(0.0f), 0.0f,
                                           isScrollUpArrow ? ((float)height) : 0.0f, false));

    g.fillRect(0, 0, width, height);

    const float hw = width * 0.5f;
    const float arrowW = height * 0.3f;
    const float y1 = height * (isScrollUpArrow ? 0.6f : 0.3f);
    const float y2 = height * (isScrollUpArrow ? 0.3f : 0.6f);

    juce::Path p;
    p.addTriangle(hw - arrowW, y1, hw + arrowW, y1, hw, y2);

    g.setColour(theme.button_on_font_colour.withAlpha(0.8f));
    g.fillPath(p);
}

void UiLookAndFeel::drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                                      const bool isSeparator, const bool isActive,
                                      const bool isHighlighted, const bool isTicked,
                                      const bool hasSubMenu, const juce::String &text,
                                      const juce::String &shortcutKeyText,
                                      const juce::Drawable *icon,
                                      const juce::Colour *const textColourToUse)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    if (isSeparator)
    {
        juce::Rectangle<int> r(area.reduced(5, 0));
        r.removeFromTop(r.getHeight() / 2 - 1);

        g.setColour(juce::Colour(0x33000000));
        g.fillRect(r.removeFromTop(1));

        g.setColour(juce::Colour(0x66ffffff));
        g.fillRect(r.removeFromTop(1));
    }
    else
    {
        juce::Rectangle<int> r(area.reduced(1));

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

        juce::Font font(getPopupMenuFont());

        const float maxFontHeight = area.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight(maxFontHeight);

        g.setFont(font);

        juce::Rectangle<float> iconArea(
            r.removeFromLeft((r.getHeight() * 5) / 4).reduced(3).toFloat());

        if (icon != nullptr)
        {
            icon->drawWithin(g, iconArea,
                             juce::RectanglePlacement::centred |
                                 juce::RectanglePlacement::onlyReduceInSize,
                             1.0f);
        }
        else if (isTicked)
        {
            const juce::Path tick(getTickShape(1.0f));
            g.fillPath(tick, tick.getTransformToScaleToFit(iconArea, true));
        }

        if (hasSubMenu)
        {
            const float arrowH = 0.6f * getPopupMenuFont().getAscent();

            const float x = (float)r.removeFromRight((int)arrowH).getX();
            const float halfH = (float)r.getCentreY();

            juce::Path p;
            p.addTriangle(x, halfH - arrowH * 0.5f, x, halfH + arrowH * 0.5f, x + arrowH * 0.6f,
                          halfH);

            g.fillPath(p);
        }

        r.removeFromRight(3);
        g.drawFittedText(text, r, juce::Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            juce::Font f2(font);
            f2.setHeight(f2.getHeight() * 0.75f);
            f2.setHorizontalScale(0.95f);
            g.setFont(f2);

            g.drawText(shortcutKeyText, r, juce::Justification::centredRight, true);
        }
    }
}
void UiLookAndFeel::drawPopupMenuSectionHeader(juce::Graphics &g, const juce::Rectangle<int> &area,
                                               const juce::String &sectionName)
{
    const SectionTheme &theme = colours.get_theme(COLOUR_THEMES::BG_THEME);
    g.setFont(getPopupMenuFont().boldened());
    g.setColour(theme.button_on_colour);

    g.drawFittedText(sectionName, area.getX() + 12, area.getY(), area.getWidth() - 16,
                     (int)(area.getHeight() * 0.8f), juce::Justification::bottomLeft, 1);
}
//==============================================================================
void UiLookAndFeel::fillTextEditorBackground(juce::Graphics &g, int /*width*/, int /*height*/,
                                             juce::TextEditor &textEditor)
{
    const bool override_theme_colour =
        textEditor.getProperties().getWithDefault(VAR_INDEX_OVERRIDE_BUTTON_COLOUR, false);

    if (override_theme_colour)
    {
        g.fillAll(colours.get_theme(BG_THEME).area_colour);
    }
    else
    {
        g.fillAll(textEditor.findColour(juce::TextEditor::backgroundColourId));
    }
}

void UiLookAndFeel::drawTextEditorOutline(juce::Graphics &g, int width, int height,
                                          juce::TextEditor &textEditor)
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

juce::CaretComponent *UiLookAndFeel::createCaretComponent(juce::Component *keyFocusOwner)
{
    return new juce::CaretComponent(keyFocusOwner);
}

//==============================================================================
void UiLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height, const bool isButtonDown,
                                 int buttonX, int buttonY, int buttonW, int buttonH,
                                 juce::ComboBox &box)
{
    SectionTheme &theme = colours.get_theme(static_cast<COLOUR_THEMES>(int(
        box.getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    const juce::Colour &user_colour = box.findColour(juce::ComboBox::backgroundColourId);
    if (box.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    const float outlineThickness = 1; // box.isEnabled() ? (isButtonDown ? 1.2f : 0.8f) : 0.3f;

    juce::Colour color_1 = user_colour.getARGB() != 0xff000000
                               ? user_colour
                               : theme.button_off_colour; // 0xff000000 default col

    g.setColour(color_1);
    g.fillRoundedRectangle(1, 1, width - 2, height - 2, 4);

    drawGlassLozenge(g, buttonX + outlineThickness, buttonY + outlineThickness,
                     buttonW - outlineThickness * 2.0f, buttonH - outlineThickness * 2.0f,
                     juce::Colour(0xff333333), outlineThickness, -1.0f, true, true, true, true);

    if (box.isEnabled() && box.hasKeyboardFocus(false))
    {
        g.setColour(theme.button_on_colour);
        g.drawRoundedRectangle(1, 1, width - 2, height - 2, 4, 1.5);
    }
    else
    {
        g.setColour(juce::Colour(0xff333333));
        g.drawRoundedRectangle(1, 1, width - 2, height - 2, 4, 1.5);
    }

    if (box.isEnabled())
    {
        const float arrowX = 0.3f;
        const float arrowH = 0.2f;

        juce::Path p;
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

juce::Font UiLookAndFeel::getComboBoxFont(juce::ComboBox &box)
{
    return defaultFont.withHeight(juce::jmin(15.0f, box.getHeight() * 0.85f));
}

juce::Label *UiLookAndFeel::createComboBoxTextBox(juce::ComboBox &)
{
    juce::Label *label = new juce::Label(juce::String(), juce::String());
    label->getProperties().set(VAR_INDEX_BUTTON_AMP, BUTTON_ON_COLOUR);
    return label;
}

void UiLookAndFeel::positionComboBoxText(juce::ComboBox &box, juce::Label &label)
{
    label.setBounds(1, 1, box.getWidth() + 3 - box.getHeight(), box.getHeight() - 2);

    label.setFont(getComboBoxFont(box));
}

//==============================================================================
juce::Font UiLookAndFeel::getLabelFont(juce::Label &label) { return defaultFont; }

void UiLookAndFeel::drawLabel(juce::Graphics &g, juce::Label &label)
{
    SectionTheme &theme = colours.get_theme(static_cast<COLOUR_THEMES>(int(
        label.getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::ARP_THEME))));
    const bool is_inverted =
        label.getProperties().getWithDefault(VAR_INDEX_COLOUR_THEME_INVERTED, false);
    const float amp = label.getProperties().getWithDefault(VAR_INDEX_BUTTON_AMP, 0);
    if (label.isOpaque())
    {
        g.fillAll(!is_inverted ? theme.area_colour : theme.area_font_colour);
    }

    if (!label.isBeingEdited())
    {
        const float height = label.getHeight();
        const float width = label.getWidth();
        const float fontHeight = FONT_SCALE * height;
        const float yIndent = IDENT_SCALE * height;
        const float cornerSize = juce::jmin(height, width) * 0.2;
        const float leftIndent = juce::jmin(fontHeight, 1.0f + cornerSize / 4);
        const float rightIndent = juce::jmin(fontHeight, 1.0f + cornerSize / 4);

        g.setFont(defaultFont.withHeight(fontHeight));
        juce::Colour col;
        if (amp != 0)
        {
            col = theme.get_color(static_cast<COLOUR_CODES>(int(amp)));
        }
        else
        {
            col = theme.area_font_colour;
        }
        g.setColour(is_inverted ? theme.area_colour : col);
        g.drawText(label.getText(),
                   juce::Rectangle<float>(leftIndent, yIndent, (width - leftIndent - rightIndent),
                                          fontHeight),
                   juce::Justification::centred, false);
    }
}

//==============================================================================
void UiLookAndFeel::drawLinearSliderBackground(juce::Graphics &g, int x, int y, int width,
                                               int height, float sliderPos, float minSliderPos,
                                               float maxSliderPos,
                                               const juce::Slider::SliderStyle /*style*/,
                                               juce::Slider &slider)
{
    SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(slider.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::FILTER_THEME))));
    if (slider.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    const bool is_midi_learn_mode = static_cast<juce::Component *>(&slider) == midi_learn_comp;

    juce::Colour col;
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
    juce::Path indent;
    if (slider.isHorizontal())
    {
        const float iy = y + height * 0.5f - sliderRadius * 0.75;
        const float ih = sliderRadius * 1.5;

        {
            g.setColour(theme.slider_bg_colour);
            indent.addRoundedRectangle(2, iy, slider.getWidth() - 4, ih - 4, 2);
            g.fillPath(indent);
            g.strokePath(indent, juce::PathStrokeType(2.5f));
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
        g.strokePath(indent, juce::PathStrokeType(2.5f));

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
            g.strokePath(indent, juce::PathStrokeType(2.5f));
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

void UiLookAndFeel::drawLinearSliderThumb(juce::Graphics &g, int x, int y, int width, int height,
                                          float sliderPos, float minSliderPos, float maxSliderPos,
                                          const juce::Slider::SliderStyle style,
                                          juce::Slider &slider)
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

void UiLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                                     float sliderPos, float minSliderPos, float maxSliderPos,
                                     const juce::Slider::SliderStyle style, juce::Slider &slider)
{
    SectionTheme &theme =
        colours.get_theme(static_cast<COLOUR_THEMES>(int(slider.getProperties().getWithDefault(
            VAR_INDEX_COLOUR_THEME, COLOUR_THEMES::DUMMY_THEME))));
    if (slider.isOpaque())
    {
        g.fillAll(theme.area_colour);
    }

    if (style == juce::Slider::LinearBar || style == juce::Slider::LinearBarVertical)
    {
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

        juce::Colour baseColour(LookAndFeelHelpers::createBaseColour(
            slider.findColour(juce::Slider::thumbColourId)
                .withMultipliedSaturation(slider.isEnabled() ? 1.0f : 0.5f),
            false, isMouseOver, isMouseOver || slider.isMouseButtonDown()));

        drawShinyButtonShape(
            g, (float)x, style == juce::Slider::LinearBarVertical ? sliderPos : (float)y,
            style == juce::Slider::LinearBarVertical ? (float)width : (sliderPos - x),
            style == juce::Slider::LinearBarVertical ? (height - sliderPos) : (float)height, 0.0f,
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

int UiLookAndFeel::getSliderThumbRadius(juce::Slider &slider)
{
    return juce::jmin(5, slider.getHeight() / 2, slider.getWidth() / 2) + 2;
}

void UiLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                                     float sliderPos, const float rotaryStartAngle,
                                     const float rotaryEndAngle, juce::Slider &slider)
{
    juce::NamedValueSet &properties = slider.getProperties();
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

    const bool is_midi_learn_mode = static_cast<juce::Component *>(&slider) == midi_learn_comp;
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

    float radius = juce::jmin(width * 0.5f, height * 0.5f) - 2.0f;
    float centreX = x + width * 0.5f;
    float centreY = y + height * 0.5f;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float rw = radius * 2.0f;
    float angle = rotaryStartAngle_ + sliderPos * (rotaryEndAngle_ - rotaryStartAngle_);

    juce::Colour SliderCol;
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
        juce::Path filledArc;
        if (slider.isOpaque())
        {
            filledArc.addPieSegment(rx, ry, rw, rw, 0, juce::MathConstants<double>::twoPi,
                                    THICKNESS);

            // g.setColour(colours.bg.withAlpha(0.2f));
            // g.setColour (SliderCol.darker (5.f).withAlpha(0.5f));
            g.setColour(theme.slider_bg_colour); //.interpolatedWith(SliderCol.darker(0.8),0.2));
            g.fillPath(filledArc);
            g.strokePath(filledArc, juce::PathStrokeType(1.5f));
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
                g.strokePath(filledArc, juce::PathStrokeType(1.5f));
            }
            else
            {
                filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle_, angle, THICKNESS);
                g.setColour(theme.slider_bg_colour);
                g.strokePath(filledArc, juce::PathStrokeType(2.5f));
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
            juce::String value_to_paint = slider.getProperties().getWithDefault(
                VAR_INDEX_VALUE_TO_SHOW, juce::String(slider.getValue()));
            const juce::String suffix = value_to_paint.fromFirstOccurrenceOf("@", false, true);
            value_to_paint = value_to_paint.upToFirstOccurrenceOf("@", false, true);

            if (suffix == "wav")
            {
                const float label_x_ident = float(width) / 3.2;
                const float label_y_ident = float(height) / 3.2;
                const float label_h = float(height) - label_y_ident * 2;
                const float label_w = float(width) - label_x_ident * 2;
                float value_as_float = value_to_paint.getFloatValue();

                juce::Path wave_path;
                if (value_as_float <= 1 && value_as_float >= 0)
                {
                    float square_weight = value_as_float;
                    for (int i = 0; i < int(label_w * 4); ++i)
                    {
                        float value_sin = std::sin((1.0f / float(label_w * 4) * i) *
                                                   juce::MathConstants<float>::twoPi);
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
                else if (value_as_float <= 2 && value_as_float >= 1)
                {
                    float saw_weight = value_as_float - 1;
                    for (int i = 0; i < int(label_w * 4); ++i)
                    {
                        float value_sin = std::sin((1.0f / float(label_w * 4) * i) *
                                                   juce::MathConstants<float>::twoPi);
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
                else if (value_as_float <= 3 && value_as_float >= 2)
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
                            ((juce::Random::getSystemRandom().nextFloat() - 0.5) * 2) * rand_weight;

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
                g.strokePath(wave_path, juce::PathStrokeType(1.f));
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

                juce::Path text_path;
                juce::GlyphArrangement glyphs;
                glyphs.addFittedText(defaultFont.withHeight(label_h), value_to_paint, label_x_ident,
                                     label_y_ident, label_w, label_h, juce::Justification::centred,
                                     1, 0.5f);

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
                                         left_right_ident, height - float(height) / 4,
                                         width - left_right_ident * 2, float(height) / 4,
                                         juce::Justification::centred, 1, 0.5f);
                    glyphs.createPath(text_path);
                    g.fillPath(text_path);
                }
            }
        }
    }
}

juce::Font UiLookAndFeel::getSliderPopupFont(juce::Slider &)
{
    return defaultFont.withHeight(17.0f);
}
int UiLookAndFeel::getSliderPopupPlacement(juce::Slider &)
{
    return juce::BubbleComponent::above | juce::BubbleComponent::below |
           juce::BubbleComponent::left | juce::BubbleComponent::right;
}
juce::PopupMenu *UiLookAndFeel::getCustomPopupMenu(juce::Slider *slider_)
{
    juce::PopupMenu *menu = new juce::PopupMenu();
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
            popup_smooth_Slider->setSliderStyle(juce::Slider::LinearHorizontal);
            popup_smooth_Slider->setTextBoxStyle(juce::Slider::NoTextBox, true, 70, 20);
            popup_smooth_Slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
            if (synth_data)
            {
                popup_smooth_Slider->setValue(int(synth_data->glide_motor_time),
                                              juce::dontSendNotification);
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
            slider_menu = std::make_unique<juce::PopupMenu>();
            // slider_menu->addSectionHeader("GLOBAL SLIDER HANDLING");

            // LINEAR
            {
                slider_menu->addSeparator();
                slider_menu->addSectionHeader("LINEAR SLIDER HANDLING");
                slider_menu->addItem(14, TRANS("Use only Rotary Sliders"), true,
                                     synth_data->only_use_rotary_sliders);
                slider_menu->addItem(11, TRANS("Velocity-Sensitive Mode (Shortcut: CTRL+drag)"),
                                     true && !synth_data->only_use_rotary_sliders,
                                     synth_data->is_linear_sliders_velocity_mode);

                slider_menu->addSeparator();
                slider_menu->addSectionHeader("Linear Velocity Acceleration");
                popup_linear_sensi_slider->setEnabled(!synth_data->only_use_rotary_sliders);
                popup_linear_sensi_slider->setRange(100, 5000, 1);
                popup_linear_sensi_slider->setTextBoxStyle(juce::Slider::NoTextBox, true, 70, 20);
                popup_linear_sensi_slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
                if (synth_data)
                {
                    popup_linear_sensi_slider->setValue(int(synth_data->sliders_linear_sensitivity),
                                                        juce::dontSendNotification);
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
                                     synth_data->sliders_in_rotary_mode &&
                                         !synth_data->is_rotary_sliders_velocity_mode);
                // menu->addItem (14, TRANS ("Use left-right dragging"), true,
                // slider_->getSliderStyle() == Slider::SliderStyle::RotaryHorizontalDrag );
                // menu->addItem (15, TRANS ("Use up-down dragging"), true,
                // slider_->getSliderStyle() == Slider::SliderStyle::RotaryVerticalDrag );
                slider_menu->addItem(16, TRANS("Use Left-Right/Up-Down Dragging"), true,
                                     !synth_data->sliders_in_rotary_mode &&
                                         !synth_data->is_rotary_sliders_velocity_mode);

                slider_menu->addSeparator();
                slider_menu->addSectionHeader("Rotary Slider Sensitivity");
                popup_rotary_sensi_slider->setRange(100, 2000, 1);
                popup_rotary_sensi_slider->setTextBoxStyle(juce::Slider::NoTextBox, true, 70, 20);
                popup_rotary_sensi_slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
                popup_rotary_sensi_slider->setValue(int(synth_data->sliders_sensitivity),
                                                    juce::dontSendNotification);
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
                animations_menu = std::make_unique<juce::PopupMenu>();
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
            popup_midi_snap_slider->setTextBoxStyle(juce::Slider::NoTextBox, true, 70, 20);
            popup_midi_snap_slider->getProperties().set(VAR_INDEX_COLOUR_THEME, BG_THEME);
            if (synth_data)
            {
                popup_midi_snap_slider->setValue(synth_data->midi_pickup_offset,
                                                 juce::dontSendNotification);
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
                help_menu = std::make_unique<juce::PopupMenu>();
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
                settings_menu = std::make_unique<juce::PopupMenu>();
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

bool UiLookAndFeel::sliderMenuCallback(const int result, juce::Slider *slider)
{
    if (slider != nullptr)
    {
        const juce::Point<int> force_mouse_pos_point =
            juce::Point<int>(slider->getScreenX() + slider->getWidth() * 0.5,
                             slider->getScreenY() + slider->getHeight() * 0.5);
        switch (result)
        {
        case 1:
            slider->setValue(slider->getProperties().getWithDefault(RETURN_VALUE_USER, 0),
                             juce::sendNotificationSync);
            break;
        case 2:
            slider->setValue(slider->getProperties().getWithDefault(RETURN_VALUE_FACTORY, 0),
                             juce::sendNotificationSync);
            break;
        case 3:
            slider->setValue(slider->getProperties().getWithDefault(RETURN_VALUE_PROGRAM, 0),
                             juce::sendNotificationSync);
            break;
        case 4:
            slider->setValue(
                slider->getProperties().getWithDefault(RETURN_VALUE_UNDO, slider->getValue()),
                juce::sendNotificationSync);
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
            slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            break;
        case 18:
            slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            break;
            /*case 19:
                /est_parameter = slider->getValue();
                break;*/
        case 20:
            if (mainwindow)
            {
                juce::Desktop::getInstance().setMousePosition(force_mouse_pos_point);
                mainwindow->show_info_popup(slider, nullptr, true);
            }
            break;
        case 25:
            if (mainwindow)
            {
                if (!force_tip)
                {
                    force_tip = std::make_unique<juce::TooltipWindow>(nullptr, 5);
                }
                juce::Desktop::getInstance().setMousePosition(force_mouse_pos_point);
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
                synth_data->set_to_stereo(!synth_data->is_stereo);
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
bool UiLookAndFeel::sliderDoubleClicked(juce::Slider *slider)
{
    if (is_global_user_return)
    {
        slider->setValue(float(slider->getProperties().getWithDefault(RETURN_VALUE_USER, 0)),
                         juce::sendNotificationSync);
    }
    else if (is_global_factory_return)
    {
        slider->setValue(float(slider->getProperties().getWithDefault(RETURN_VALUE_FACTORY, 0)),
                         juce::sendNotificationSync);
    }
    else if (is_global_program_return)
    {
        slider->setValue(float(slider->getProperties().getWithDefault(RETURN_VALUE_PROGRAM, 0)),
                         juce::sendNotificationSync);
    }
    else
    {
        const float current_value = slider->getValue();
        slider->setValue(
            float(slider->getProperties().getWithDefault(RETURN_VALUE_UNDO, current_value)),
            juce::sendNotificationSync);
        slider->getProperties().set(RETURN_VALUE_UNDO, current_value);
    }

    return true;
}
void UiLookAndFeel::sliderValueChanged(juce::Slider *sliderThatWasMoved)
{
    if (sliderThatWasMoved == popup_smooth_Slider.get())
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
    else if (sliderThatWasMoved == popup_linear_sensi_slider.get())
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
    else if (sliderThatWasMoved == popup_rotary_sensi_slider.get())
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
    else if (sliderThatWasMoved == popup_midi_snap_slider.get())
    {
        synth_data->midi_pickup_offset = sliderThatWasMoved->getValue();
    }
}

//==============================================================================
juce::Rectangle<int> UiLookAndFeel::getTooltipBounds(const juce::String &tipText,
                                                     juce::Point<int> screenPos,
                                                     juce::Rectangle<int> parentArea)
{
    const juce::TextLayout tl(
        LookAndFeelHelpers::layoutTooltipText(tipText, juce::Colours::black, defaultFont));

    const int w = (int)(tl.getWidth() + 30.0f);
    const int h = (int)(tl.getHeight() + 20.0f);

    return juce::Rectangle<int>(
               screenPos.x > parentArea.getCentreX() ? screenPos.x - (w + 12) : screenPos.x + 24,
               screenPos.y > parentArea.getCentreY() ? screenPos.y - (h + 6) : screenPos.y + 6, w,
               h)
        .constrainedWithin(parentArea);
}

void UiLookAndFeel::drawTooltip(juce::Graphics &g, const juce::String &text, int width, int height)
{
    g.fillAll(findColour(juce::TooltipWindow::backgroundColourId));

    // #if ! JUCE_MAC // The mac windows already have a non-optional 1 pix outline, so don't double
    //  it here..
    g.setColour(findColour(juce::TooltipWindow::outlineColourId));
    g.drawRect(0, 0, width, height, 1);
    // #endif

    juce::Rectangle<float> rect = juce::Rectangle<float>((float)width, (float)height);
    juce::TextLayout tl = LookAndFeelHelpers::layoutTooltipText(
        text, findColour(juce::TooltipWindow::textColourId), defaultFont);
    float center_rest = rect.getWidth() - tl.getWidth();
    rect.setX(center_rest / 2);
    tl.draw(g, rect);
}

//==============================================================================
void UiLookAndFeel::drawCornerResizer(juce::Graphics &g, int w, int h, bool /*isMouseOver*/,
                                      bool /*isMouseDragging*/)
{
    const float lineThickness = juce::jmin(w, h) * 0.075f;

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        g.setColour(colours.get_theme(BG_THEME).area_font_colour);
        g.drawLine(w * i + lineThickness, h + 1.0f, w + 1.0f, h * i + lineThickness, lineThickness);
    }
}

//==============================================================================
void UiLookAndFeel::fillResizableWindowBackground(juce::Graphics &g, int /*w*/, int /*h*/,
                                                  const juce::BorderSize<int> & /*border*/,
                                                  juce::ResizableWindow &window)
{
}

void UiLookAndFeel::drawResizableWindowBorder(juce::Graphics &g, int /*w*/, int /*h*/,
                                              const juce::BorderSize<int> & /*border*/,
                                              juce::ResizableWindow &)
{
    // g.fillAll(Colours::black);
}

void UiLookAndFeel::drawDocumentWindowTitleBar(juce::DocumentWindow &window, juce::Graphics &g,
                                               int w, int h, int titleSpaceX, int titleSpaceW,
                                               const juce::Image *icon, bool drawTitleTextOnLeft)
{
    const bool isActive = window.isActiveWindow();

    g.fillAll(window.getBackgroundColour().contrasting(isActive ? 0.0f : 0.25f));

    juce::Font font = defaultFont.withHeight(h * 0.65f);
    g.setFont(font);

    int textW = font.getStringWidth(window.getName());
    int iconW = 0;
    int iconH = 0;

    if (icon != nullptr)
    {
        iconH = (int)font.getHeight();
        iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
    }

    textW = juce::jmin(titleSpaceW, textW + iconW);
    int textX = drawTitleTextOnLeft ? titleSpaceX : juce::jmax(titleSpaceX, (w - textW) / 2);

    if (textX + textW > titleSpaceX + titleSpaceW)
        textX = titleSpaceX + titleSpaceW - textW;

    if (icon != nullptr)
    {
        g.setOpacity(isActive ? 1.0f : 0.6f);
        g.drawImageWithin(*icon, textX, (h - iconH) / 2, iconW, iconH,
                          juce::RectanglePlacement::centred, false);
        textX += iconW;
        textW -= iconW;
    }

    if (window.isColourSpecified(juce::DocumentWindow::textColourId) ||
        isColourSpecified(juce::DocumentWindow::textColourId))
        g.setColour(window.findColour(juce::DocumentWindow::textColourId));
    else
        g.setColour(window.getBackgroundColour().contrasting(isActive ? 0.7f : 0.4f));

    g.drawText(window.getName(), textX, 0, textW, h, juce::Justification::centredLeft, true);
}

//==============================================================================
class UiLookAndFeel::GlassWindowButton : public juce::Button
{
  public:
    GlassWindowButton(const juce::String &name, juce::Colour col, const juce::Path &normalShape_,
                      const juce::Path &toggledShape_) noexcept
        : juce::Button(name), colour(col), normalShape(normalShape_), toggledShape(toggledShape_)
    {
    }

    //==============================================================================
    void paintButton(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown) override
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

        g.setColour(juce::Colour::greyLevel(0.6f).withAlpha(
            alpha)); // , 0, y + diam, Colour::greyLevel (0.6f).withAlpha (alpha), 0, y, false
        g.fillEllipse(x, y, diam, diam);

        x += 2.0f;
        y += 2.0f;
        diam -= 4.0f;

        UiLookAndFeel::drawGlassSphere(g, x, y, diam, colour, 1.0f);

        juce::Path &p = getToggleState() ? toggledShape : normalShape;

        const juce::AffineTransform t(p.getTransformToScaleToFit(x + diam * 0.3f, y + diam * 0.3f,
                                                                 diam * 0.4f, diam * 0.4f, true));

        g.setColour(juce::Colours::black.withAlpha(alpha * 0.6f));
        g.fillPath(p, t);
    }

  private:
    juce::Colour colour;
    juce::Path normalShape, toggledShape;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlassWindowButton)
};

juce::Button *UiLookAndFeel::createDocumentWindowButton(int buttonType)
{
    juce::Path shape;
    const float crossThickness = 0.25f;

    if (buttonType == juce::DocumentWindow::closeButton)
    {
        shape.addLineSegment(juce::Line<float>(0.0f, 0.0f, 1.0f, 1.0f), crossThickness * 1.4f);
        shape.addLineSegment(juce::Line<float>(1.0f, 0.0f, 0.0f, 1.0f), crossThickness * 1.4f);

        return new GlassWindowButton("close", juce::Colour(0xff991100), shape, shape);
    }

    if (buttonType == juce::DocumentWindow::minimiseButton)
    {
        shape.addLineSegment(juce::Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        return new GlassWindowButton("minimise", juce::Colour(0xffdfce89), shape, shape);
    }

    if (buttonType == juce::DocumentWindow::maximiseButton)
    {
        shape.addLineSegment(juce::Line<float>(0.5f, 0.0f, 0.5f, 1.0f), crossThickness);
        shape.addLineSegment(juce::Line<float>(0.0f, 0.5f, 1.0f, 0.5f), crossThickness);

        juce::Path fullscreenShape;
        fullscreenShape.startNewSubPath(45.0f, 100.0f);
        fullscreenShape.lineTo(0.0f, 100.0f);
        fullscreenShape.lineTo(0.0f, 0.0f);
        fullscreenShape.lineTo(100.0f, 0.0f);
        fullscreenShape.lineTo(100.0f, 45.0f);
        fullscreenShape.addRectangle(45.0f, 45.0f, 100.0f, 100.0f);
        juce::PathStrokeType(30.0f).createStrokedPath(fullscreenShape, fullscreenShape);

        return new GlassWindowButton("maximise", juce::Colour(0xffdfce89), shape, fullscreenShape);
    }

    jassertfalse;
    return nullptr;
}

void UiLookAndFeel::positionDocumentWindowButtons(juce::DocumentWindow &, int titleBarX,
                                                  int titleBarY, int titleBarW, int titleBarH,
                                                  juce::Button *minimiseButton,
                                                  juce::Button *maximiseButton,
                                                  juce::Button *closeButton,
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
void UiLookAndFeel::drawShinyButtonShape(juce::Graphics &g, float x, float y, float w, float h,
                                         float maxCornerSize, const juce::Colour &baseColour,
                                         const float strokeWidth, const bool flatOnLeft,
                                         const bool flatOnRight, const bool flatOnTop,
                                         const bool flatOnBottom) noexcept
{
    if (w <= strokeWidth * 1.1f || h <= strokeWidth * 1.1f)
        return;

    const float cs = juce::jmin(maxCornerSize, w * 0.5f, h * 0.5f);

    juce::Path outline;
    outline.addRoundedRectangle(x, y, w, h, cs, cs, !(flatOnLeft || flatOnTop),
                                !(flatOnRight || flatOnTop), !(flatOnLeft || flatOnBottom),
                                !(flatOnRight || flatOnBottom));

    juce::ColourGradient cg(baseColour, 0.0f, y, baseColour.overlaidWith(juce::Colour(0x070000ff)),
                            0.0f, y + h, false);

    cg.addColour(0.5, baseColour.overlaidWith(juce::Colour(0x33ffffff)));
    cg.addColour(0.51, baseColour.overlaidWith(juce::Colour(0x110000ff)));

    g.setGradientFill(cg);
    g.fillPath(outline);

    g.setColour(juce::Colour(0x80000000));
    g.strokePath(outline, juce::PathStrokeType(strokeWidth));
}

//==============================================================================
void UiLookAndFeel::drawGlassSphere(juce::Graphics &g, const float x, const float y,
                                    const float diameter, const juce::Colour &colour,
                                    const float outlineThickness) noexcept
{
    if (diameter <= outlineThickness)
        return;

    juce::Path p;
    p.addEllipse(x, y, diameter, diameter);

    {
        g.setColour(colour);
        g.fillPath(p);
    }

    // g.setGradientFill (ColourGradient (Colours::white, 0, y + diameter * 0.06f,
    // Colours::transparentWhite, 0, y + diameter * 0.3f, false));
    // g.fillEllipse (x + diameter * 0.2f, y + diameter * 0.05f, diameter * 0.6f, diameter * 0.4f);

    juce::ColourGradient cg(
        juce::Colours::transparentBlack, x + diameter * 0.5f, y + diameter * 0.5f,
        juce::Colours::black.withAlpha(0.5f * outlineThickness * colour.getFloatAlpha()), x,
        y + diameter * 0.5f, true);

    // cg.addColour (0.7, Colours::transparentBlack);
    // cg.addColour (0.8, Colours::black.withAlpha (0.1f * outlineThickness));

    // g.setGradientFill (cg);
    // g.fillPath (p);

    g.setColour(juce::Colours::black.withAlpha(0.5f * colour.getFloatAlpha()));
    g.drawEllipse(x, y, diameter, diameter, outlineThickness);
}

//==============================================================================
void UiLookAndFeel::drawGlassPointer(juce::Graphics &g, const float x, const float y,
                                     const float diameter, const juce::Colour &colour,
                                     const float outlineThickness, const int direction) noexcept
{
    if (diameter <= outlineThickness)
        return;

    juce::Path p;
    p.startNewSubPath(x + diameter * 0.5f, y);
    p.lineTo(x + diameter, y + diameter * 0.6f);
    p.lineTo(x + diameter, y + diameter);
    p.lineTo(x, y + diameter);
    p.lineTo(x, y + diameter * 0.6f);
    p.closeSubPath();

    p.applyTransform(
        juce::AffineTransform::rotation(direction * (juce::MathConstants<float>::pi * 0.5f),
                                        x + diameter * 0.5f, y + diameter * 0.5f));

    {
        juce::ColourGradient cg(juce::Colours::white.overlaidWith(colour.withMultipliedAlpha(0.3f)),
                                0, y,
                                juce::Colours::white.overlaidWith(colour.withMultipliedAlpha(0.3f)),
                                0, y + diameter, false);

        cg.addColour(0.4, juce::Colours::white.overlaidWith(colour));

        g.setGradientFill(cg);
        g.fillPath(p);
    }

    juce::ColourGradient cg(
        juce::Colours::transparentBlack, x + diameter * 0.5f, y + diameter * 0.5f,
        juce::Colours::black.withAlpha(0.5f * outlineThickness * colour.getFloatAlpha()),
        x - diameter * 0.2f, y + diameter * 0.5f, true);

    cg.addColour(0.5, juce::Colours::transparentBlack);
    cg.addColour(0.7, juce::Colours::black.withAlpha(0.07f * outlineThickness));

    g.setGradientFill(cg);
    g.fillPath(p);

    g.setColour(juce::Colours::black.withAlpha(0.5f * colour.getFloatAlpha()));
    g.strokePath(p, juce::PathStrokeType(outlineThickness));
}

//==============================================================================
void UiLookAndFeel::drawGlassLozenge(juce::Graphics &g, const float x, const float y,
                                     const float width, const float height,
                                     const juce::Colour &colour, const float outlineThickness,
                                     const float cornerSize, const bool flatOnLeft,
                                     const bool flatOnRight, const bool flatOnTop,
                                     const bool flatOnBottom) noexcept
{
    juce::Path outline;
    outline.addRoundedRectangle(x, y, width, height, 2);

    {
        g.setColour(colour);
        g.fillPath(outline);
    }
}
