# CMake file for the Juce 6 port of Monique (underway)

cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "Build for 10.12")

project(MoniqueMonosynth VERSION 1.2.0 LANGUAGES C CXX ASM)

# cmake options
option(MONIQUE_COPY_PLUGIN_AFTER_BUILD "Copy JUCE Plugins after built" OFF)

# Set ourselves up for fpic C++17 all platforms
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(libs/JUCE)


juce_add_plugin(MoniqueMonosynth
        VERSION "1.2"
        BUNDLE_ID "com.monoplugs.monique"
        COMPANY_NAME "Monoplugs"
        COMPANY_COPYRIGHT "Monoplugs 2021"
        COMPANY_WEBSITE "http://monique.monoplugs.com"
        COMPANY_EMAIL "info@monoplugs.com"

        FORMATS VST3 AU Standalone

        PLUGIN_NAME "Monique"
        DESCRIPTION "Monophonic Unique Synthesizer"
        PLUGIN_MANUFACTURER_CODE "MoPl"
        PLUGIN_CODE "moni"
        AAX_IDENTIFIER "com.monoplugs.monique-synthesizer"
        AU_EXPORT_PREFIX "moniAU"

        COPY_PLUGIN_AFTER_BUILD ${MONIQUE_COPY_PLUGIN_AFTER_BUILD}
)

juce_generate_juce_header(MoniqueMonosynth)

target_compile_definitions(MoniqueMonosynth
  PUBLIC
        DROWAUDIO_USE_CURL=disabled
        JUCE_ALSA=1
        JUCE_JACK=1

        JUCE_ASIO=0
        JUCE_DIRECTSOUND=1
        JUCE_WASAPI=1

        JUCE_USE_CURL=0
        JUCE_WEB_BROWSER=0
        JUCE_VST3_CAN_REPLACE_VST2=0

        # Note if you are building the code under MIT you need a license to use JUCE to set this to 0
        JUCE_DISPLAY_SPLASH_SCREEN=0

        # These force it into "plugin" mode even for the juce 6 standalone
        AUTO_STANDALONE=0
        IS_STANDALONE=0
        IS_PLUGIN=1
)

target_sources(MoniqueMonosynth
  PRIVATE
    Source/monique_core_Datastructures.cpp
    Source/monique_core_Parameters.cpp
    Source/monique_core_Processor.cpp
    Source/monique_core_Synth.cpp
    Source/monique_ui_Activate.cpp
    Source/monique_ui_AmpPainter.cpp
    Source/monique_ui_Credits.cpp
    Source/monique_ui_Demo.cpp
    Source/monique_ui_DragPad.cpp
    Source/monique_ui_ENVPopup.cpp
    Source/monique_ui_GlobalSettings.cpp
    Source/monique_ui_LookAndFeel.cpp
    Source/monique_ui_MFOPopup.cpp
    Source/monique_ui_MIDIIO.cpp
    Source/monique_ui_MainWindow.cpp
    Source/monique_ui_MidiLearnPopup.cpp
    Source/monique_ui_ModulationSlider.cpp
    Source/monique_ui_Morph.cpp
    Source/monique_ui_OptionPopup.cpp
    Source/monique_ui_Overlay.cpp
    Source/monique_ui_Playback.cpp
    Source/monique_ui_Refresher.cpp
    Source/monique_ui_SegmentedMeter.cpp
    Source/mono_AudioDeviceManager.cpp
)

if (APPLE OR UNIX)
    # these all need fixing obviously
    target_compile_options(MoniqueMonosynth PUBLIC
            -Wno-deprecated-declarations
            -Wno-float-conversion
            -Wno-sign-conversion
            -Wno-shadow
            )
endif()

juce_add_binary_data(MoniqueMonosynth_BinaryData
  SOURCES
    Files/A.zip
    Files/DARK.mcol
    "Files/FACTORTY DEFAULT.mlprog"
    Files/Themes.zip
    Fonts/SourceCodePro-Medium.otf
    Fonts/lato/Lato-Semibold.ttf
    Icon/monique_512x512.png
    Icon/monique_desktop_icon_1024x1024.png
)

target_link_libraries(MoniqueMonosynth
  PRIVATE
    MoniqueMonosynth_BinaryData
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_cryptography
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
