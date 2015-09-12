
#include "App.h"

#if !( defined IS_PLUGIN || defined IS_STANDALONE)
// JUST A HACK TO BROWSE FILES IN KDEV - KDEV HAD A PROBLEM WITH... - ANYWAY - THE COMPILER WILL NEVER SEE THIS!
#include "../../JUCE/modules/juce_audio_basics/juce_audio_basics.h"
#include "../../JUCE/modules/juce_audio_devices/juce_audio_devices.h"
#include "../../JUCE/modules/juce_audio_formats/juce_audio_formats.h"
#include "../../JUCE/modules/juce_audio_plugin_client/juce_audio_plugin_client.h"
#include "../../JUCE/modules/juce_audio_processors/juce_audio_processors.h"
#include "../../JUCE/modules/juce_audio_utils/juce_audio_utils.h"
#include "../../JUCE/modules/juce_core/juce_core.h"
#include "../../JUCE/modules/juce_core/containers/juce_Array.h"
#include "../../JUCE/modules/juce_cryptography/juce_cryptography.h"
#include "../../JUCE/modules/juce_data_structures/juce_data_structures.h"
#include "../../JUCE/modules/juce_events/juce_events.h"
#include "../../JUCE/modules/juce_graphics/juce_graphics.h"
#include "../../JUCE/modules/juce_gui_basics/juce_gui_basics.h"
#include "../../JUCE/modules/juce_gui_extra/juce_gui_extra.h"

#include "monique_ui_Refresher.h"

#undef COLD
#define COLD
#undef restrict
#define restrict
#undef noexcept
#define noexcept
#define IS_STANDALONE
#endif
