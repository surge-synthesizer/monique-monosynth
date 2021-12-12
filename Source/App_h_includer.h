
#include "App.h"

#ifndef IS_STANDALONE
#ifndef IS_PLUGIN
#define DEVELOPMENT
#endif
#endif

#ifndef IS_PLUGIN
#ifndef IS_STANDALONE
#define DEVELOPMENT
#endif
#endif


#ifdef DEVELOPMENT
#error 123
// JUST A HACK TO BROWSE FILES IN KDEV - KDEV HAD A PROBLEM WITH... - ANYWAY - THE COMPILER WILL NEVER SEE THIS!
#include <JuceHeader.h>

#include "monique_ui_Refresher.h"

#undef COLD
#define COLD
#undef restrict
#define restrict
#undef noexcept
#define noexcept
#define IS_STANDALONE
#endif
