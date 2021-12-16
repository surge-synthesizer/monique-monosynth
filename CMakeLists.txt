# CMake file for the Juce 6 port of Monique (underway)

cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "Build for 10.12")

project(MoniqueMonosynth VERSION 1.2.0 LANGUAGES C CXX ASM)

# cmake options
option(MONIQUE_COPY_PLUGIN_AFTER_BUILD "Copy JUCE Plugins after built" OFF)
option(MONIQUE_RELIABLE_VERSION_INFO "Update version info on every build (off: generate only at configuration time)" ON)

# Set ourselves up for fpic C++17 all platforms
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MONIQUE_JUCE_PATH "${CMAKE_SOURCE_DIR}/libs/JUCE" CACHE STRING "Path to JUCE library source tree")

add_subdirectory(${MONIQUE_JUCE_PATH} ${CMAKE_BINARY_DIR}/JUCE EXCLUDE_FROM_ALL)
add_subdirectory(libs/oddsound-mts)

set(MONIQUE_JUCE_FORMATS VST3 Standalone)

if (APPLE)
  list(APPEND MONIQUE_JUCE_FORMATS AU)
endif()

if(MONIQUE_CLAP_ROOT)
  message( STATUS "Gathering CLAP from ${MONIQUE_CLAP_ROOT}")
  if (NOT EXISTS "${MONIQUE_JUCE_PATH}/modules/juce_audio_plugin_client/juce_audio_plugin_client_CLAP.cpp")
    message(FATAL_ERROR "To use CLAP you need to both set MONIQUE_CLAP_ROOT and have MONIQUE_JUCE_PATH point at a clap-enabled juce")
  endif()
  juce_set_clap_sdk_path("${MONIQUE_CLAP_ROOT}/clap" "${MONIQUE_CLAP_ROOT}/clap-helpers")
  list(APPEND MONIQUE_JUCE_FORMATS CLAP)
endif()

message(STATUS "Building Monique in formats: ${MONIQUE_JUCE_FORMATS}" )

juce_add_plugin(MoniqueMonosynth
        VERSION "1.2"
        BUNDLE_ID "org.surge-synth-team.monique"
        COMPANY_NAME "Surge Synth Team"
        PLUGIN_MANUFACTURER_CODE "VmbA"
        COMPANY_COPYRIGHT "Copyright Thomas Arndt and Authors in Github, 2018-2021"
        COMPANY_WEBSITE "http://surge-synth-team.org"

        ICON_BIG "${CMAKE_SOURCE_DIR}/Icon/monique_512x512.png"

        FORMATS ${MONIQUE_JUCE_FORMATS}

        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE

        PLUGIN_NAME "Monique"
        DESCRIPTION "Monophonic Unique Synthesizer"

        PLUGIN_CODE "moni"
        AAX_IDENTIFIER "com.monoplugs.monique-synthesizer"
        AU_EXPORT_PREFIX "moniAU"

        COPY_PLUGIN_AFTER_BUILD ${MONIQUE_COPY_PLUGIN_AFTER_BUILD}
)

juce_generate_juce_header(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}
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

        # Enables old code paths with MIDI IO handling
        IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING=0
)

target_sources(${PROJECT_NAME}
  PRIVATE
    Source/monique_core_Datastructures.cpp
    Source/monique_core_Parameters.cpp
    Source/monique_core_Processor.cpp
    Source/monique_core_Synth.cpp
    Source/monique_ui_AmpPainter.cpp
    Source/monique_ui_Credits.cpp
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

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    MoniqueMonosynth_BinaryData
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags

    monique::oddsound-mts
)


if (APPLE)
    # these all need fixing obviously. The strategy I've used on mac here is
    # 1: Turn off the juce suggested warning flags
    # 2: Turn on werror (mac only)
    # 3: Turn off every no-blah which stops me building (and apply these on linux too)
    #
    # Then one by one we should document each of these and decide if we want to
    # keep them or fix them. Most probably fix.
    message(STATUS "Turning off a collection of legitimate compiler warnings" )
    target_compile_options(${PROJECT_NAME} PUBLIC
            -Werror
            -Wno-deprecated-declarations
            -Wno-float-conversion
            -Wno-sign-conversion
            -Wno-unused-value
            -Wno-return-type
            -Wno-literal-conversion
            )
elseif(UNIX)
    # FIXME - I should be more careful about assuming unix-not-apple == gcc
    message(STATUS "Turning off a collection of legitimate compiler warnings" )
    target_compile_options(${PROJECT_NAME} PUBLIC
            -Werror
            -Wno-deprecated-declarations
            -Wno-float-conversion
            -Wno-sign-conversion
            -Wno-unused-value
            -Wno-return-type
            -Wno-literal-conversion
            -Wno-shadow

            # here's the ones we don't have on macos/clang
            -Wno-enum-compare
            )
else()
    # FIXME - and simiilarly about assuming windows == MSVC
    message(STATUS "Turning off a collection of legitimate compiler warnings" )
    target_compile_options(${PROJECT_NAME} PUBLIC
            /WX # Warnings are errors
            /W3 # and we show lots of them

            # These ones need fixing for sure
            /wd4804 # unsafe use of bool
            /wd4805 # unsafe use of bool and float
            /wd4715 # return values
            /wd4996 # deprecation
            /wd4099 # first see as struct vs class

            # these ones are probably ok in our context
            /wd4244 # double to float
            /wd4305 # float to double

            /permissive-  # allow the full spec, including 'not' and 'and keywords
            )

endif()


# version generation
if(MONIQUE_RELIABLE_VERSION_INFO)
    add_custom_target(version-info BYPRODUCTS ${CMAKE_BINARY_DIR}/geninclude/version.cpp
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -D CMAKE_PROJECT_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
            -D CMAKE_PROJECT_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
            -D MONIQUESRC=${CMAKE_SOURCE_DIR} -D MONIQUEBLD=${CMAKE_BINARY_DIR}
            -D AZURE_PIPELINE=${AZURE_PIPELINE}
            -D WIN32=${WIN32}
            -D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
            -D CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}
            -D CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}
            -P ${CMAKE_SOURCE_DIR}/cmake/versiontools.cmake
            )
    add_dependencies(${PROJECT_NAME} version-info)
else()
    set(MONIQUESRC ${CMAKE_SOURCE_DIR})
    set(MONIQUEBLD ${CMAKE_BINARY_DIR})
    include(${CMAKE_SOURCE_DIR}/cmake/versiontools.cmake)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Source)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/geninclude/version.cpp)

include(${CMAKE_SOURCE_DIR}/cmake/basic_installer.cmake)
