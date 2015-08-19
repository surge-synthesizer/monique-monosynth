/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_AF78624DB5AE9BA__
#define __JUCE_HEADER_AF78624DB5AE9BA__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class UiDragPad;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UiEditorMorph  : public Component,
                       public mono_UiRefreshable,
                       public ButtonListener,
                       public ComboBoxListener
{
public:
    //==============================================================================
    UiEditorMorph ();
    ~UiEditorMorph();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    bool keyPressed (const KeyPress& key);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void refresh() override;
    Array< int > bank_start_indexs;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Component> hit_area_3;
    ScopedPointer<Component> hit_area_1;
    ScopedPointer<Component> hit_area_2;
    ScopedPointer<Component> hit_area_through;
    ScopedPointer<Label> label_g_1;
    ScopedPointer<Label> label_1;
    ScopedPointer<Label> label_2;
    ScopedPointer<ToggleButton> toggle_all_1;
    ScopedPointer<Label> label_3;
    ScopedPointer<ToggleButton> toggle_filters_1;
    ScopedPointer<ToggleButton> toggle_filter_1_1;
    ScopedPointer<Label> label_4;
    ScopedPointer<ToggleButton> toggle_filter_2_1;
    ScopedPointer<Label> label_5;
    ScopedPointer<ToggleButton> toggle_filter_3_1;
    ScopedPointer<Label> label_6;
    ScopedPointer<Label> label_7;
    ScopedPointer<ToggleButton> toggle_oscs_1;
    ScopedPointer<ToggleButton> toggle_osc_1_1;
    ScopedPointer<Label> label_8;
    ScopedPointer<ToggleButton> toggle_osc_2_1;
    ScopedPointer<Label> label_9;
    ScopedPointer<ToggleButton> toggle_osc_3_1;
    ScopedPointer<Label> label_10;
    ScopedPointer<ToggleButton> toggle_fm_1;
    ScopedPointer<Label> label_11;
    ScopedPointer<ToggleButton> toggle_effects_1;
    ScopedPointer<Label> label_15;
    ScopedPointer<ToggleButton> toggle_arp_1;
    ScopedPointer<Label> label_16;
    ScopedPointer<ToggleButton> toggle_arp_notes_1;
    ScopedPointer<Label> label_17;
    ScopedPointer<ToggleButton> toggle_arp_velocity_1;
    ScopedPointer<Label> label_18;
    ScopedPointer<ToggleButton> toggle_arp_glide_shuffle_1;
    ScopedPointer<Label> label_20;
    ScopedPointer<ToggleButton> toggle_eq_1;
    ScopedPointer<Label> label_g_2;
    ScopedPointer<Label> label_g_3;
    ScopedPointer<Label> label_g_4;
    ScopedPointer<ComboBox> combo_morph_group_1;
    ScopedPointer<ComboBox> combo_morph_group_2;
    ScopedPointer<ComboBox> combo_morph_group_3;
    ScopedPointer<ComboBox> combo_morph_group_4;
    ScopedPointer<ComboBox> combo_morph_group_5;
    ScopedPointer<ComboBox> combo_morph_group_6;
    ScopedPointer<ComboBox> combo_morph_group_7;
    ScopedPointer<ComboBox> combo_morph_group_8;
    ScopedPointer<Label> label_19;
    ScopedPointer<Label> label_21;
    ScopedPointer<ToggleButton> toggle_all_2;
    ScopedPointer<ToggleButton> toggle_filters_2;
    ScopedPointer<ToggleButton> toggle_filter_1_2;
    ScopedPointer<ToggleButton> toggle_filter_2_2;
    ScopedPointer<ToggleButton> toggle_filter_3_2;
    ScopedPointer<ToggleButton> toggle_oscs_2;
    ScopedPointer<ToggleButton> toggle_osc_1_2;
    ScopedPointer<ToggleButton> toggle_osc_2_2;
    ScopedPointer<ToggleButton> toggle_osc_3_2;
    ScopedPointer<ToggleButton> toggle_fm_2;
    ScopedPointer<ToggleButton> toggle_effects_2;
    ScopedPointer<ToggleButton> toggle_arp_2;
    ScopedPointer<ToggleButton> toggle_arp_notes_2;
    ScopedPointer<ToggleButton> toggle_arp_velocity_2;
    ScopedPointer<ToggleButton> toggle_arp_glide_shuffle_2;
    ScopedPointer<ToggleButton> toggle_eq_2;
    ScopedPointer<ToggleButton> toggle_all_3;
    ScopedPointer<ToggleButton> toggle_filters_3;
    ScopedPointer<ToggleButton> toggle_filter_1_3;
    ScopedPointer<ToggleButton> toggle_filter_2_3;
    ScopedPointer<ToggleButton> toggle_filter_3_3;
    ScopedPointer<ToggleButton> toggle_oscs_3;
    ScopedPointer<ToggleButton> toggle_osc_1_3;
    ScopedPointer<ToggleButton> toggle_osc_2_3;
    ScopedPointer<ToggleButton> toggle_osc_3_3;
    ScopedPointer<ToggleButton> toggle_fm_3;
    ScopedPointer<ToggleButton> toggle_effects_3;
    ScopedPointer<ToggleButton> toggle_arp_3;
    ScopedPointer<ToggleButton> toggle_arp_notes_3;
    ScopedPointer<ToggleButton> toggle_arp_velocity_3;
    ScopedPointer<ToggleButton> toggle_arp_glide_shuffle_3;
    ScopedPointer<ToggleButton> toggle_eq_3;
    ScopedPointer<ToggleButton> toggle_all_4;
    ScopedPointer<ToggleButton> toggle_filters_4;
    ScopedPointer<ToggleButton> toggle_filter_1_4;
    ScopedPointer<ToggleButton> toggle_filter_2_4;
    ScopedPointer<ToggleButton> toggle_filter_3_4;
    ScopedPointer<ToggleButton> toggle_oscs_4;
    ScopedPointer<ToggleButton> toggle_osc_1_4;
    ScopedPointer<ToggleButton> toggle_osc_2_4;
    ScopedPointer<ToggleButton> toggle_osc_3_4;
    ScopedPointer<ToggleButton> toggle_fm_4;
    ScopedPointer<ToggleButton> toggle_effects_4;
    ScopedPointer<ToggleButton> toggle_arp_4;
    ScopedPointer<ToggleButton> toggle_arp_notes_4;
    ScopedPointer<ToggleButton> toggle_arp_velocity_4;
    ScopedPointer<ToggleButton> toggle_arp_glide_shuffle_4;
    ScopedPointer<ToggleButton> toggle_eq_4;
    ScopedPointer<Label> label_12;
    ScopedPointer<ToggleButton> toggle_main_1;
    ScopedPointer<ToggleButton> toggle_main_2;
    ScopedPointer<ToggleButton> toggle_main_3;
    ScopedPointer<ToggleButton> toggle_main_4;
    ScopedPointer<Label> label_13;
    ScopedPointer<TextButton> button_set_left_1;
    ScopedPointer<TextButton> button_set_left_2;
    ScopedPointer<TextButton> button_set_left_3;
    ScopedPointer<TextButton> button_set_left_4;
    ScopedPointer<Label> label_14;
    ScopedPointer<TextButton> button_set_right_1;
    ScopedPointer<TextButton> button_set_right_2;
    ScopedPointer<TextButton> button_set_right_3;
    ScopedPointer<TextButton> button_set_right_4;
    ScopedPointer<TextButton> button_set_all_from_current;
    ScopedPointer<UiDragPad> track_pad;
    ScopedPointer<Label> label_23;
    ScopedPointer<ToggleButton> toggle_arp_switchs_1;
    ScopedPointer<ToggleButton> toggle_arp_switchs_2;
    ScopedPointer<ToggleButton> toggle_arp_switchs_3;
    ScopedPointer<ToggleButton> toggle_arp_switchs_4;
    ScopedPointer<TextButton> close;
    Path internalPath1;
    Path internalPath2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiEditorMorph)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_AF78624DB5AE9BA__
