# CMake file for the Juce 6 port of Monique (underway)

cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "Build for 10.12")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(MoniqueMonosynth VERSION 1.2.0 LANGUAGES C CXX ASM)

# cmake options
option(MONIQUE_COPY_PLUGIN_AFTER_BUILD "Copy JUCE Plugins after built" OFF)
option(MONIQUE_RELIABLE_VERSION_INFO "Update version info on every build (off: generate only at configuration time)" ON)

# Set ourselves up for fpic C++17 all platforms
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Calculate bitness
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
message(STATUS "Targeting ${BITS}-bit configuration")

set(MONIQUE_JUCE_PATH "${CMAKE_SOURCE_DIR}/libs/JUCE" CACHE STRING "Path to JUCE library source tree")
add_subdirectory(${MONIQUE_JUCE_PATH} ${CMAKE_BINARY_DIR}/JUCE EXCLUDE_FROM_ALL)
add_subdirectory(libs/clap-juce-extensions EXCLUDE_FROM_ALL)
add_subdirectory(libs/oddsound-mts)

set(MONIQUE_JUCE_FORMATS CLAP VST3 Standalone)

if(APPLE)
  list(APPEND MONIQUE_JUCE_FORMATS AU)
endif()

if(DEFINED ENV{VST2SDK_DIR})
  file(TO_CMAKE_PATH "$ENV{VST2SDK_DIR}" JUCE_VST2_DIR)
  juce_set_vst2_sdk_path(${JUCE_VST2_DIR})
  list(APPEND MONIQUE_JUCE_FORMATS VST)
  message(STATUS "VST2 SDK found at $ENV{VST2SDK_DIR}")
  # VST2 headers are invalid UTF-8
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/wd4828>)
endif()

if(BUILD_LV2)
  message(STATUS "Activating LV2 for testing purpose")
  list(APPEND MONIQUE_JUCE_FORMATS LV2)
endif()

message(STATUS "Building Monique in formats: ${MONIQUE_JUCE_FORMATS}" )

juce_add_plugin(MoniqueMonosynth
  VERSION "1.2"
  BUNDLE_ID "org.surge-synth-team.monique"
  COMPANY_NAME "Surge Synth Team"
  PLUGIN_MANUFACTURER_CODE "VmbA"
  COMPANY_COPYRIGHT "Copyright Thomas Arndt and Authors in Github, 2018-2021"
  COMPANY_WEBSITE "https://surge-synth-team.org"

  ICON_BIG "${CMAKE_SOURCE_DIR}/resources/icon/monique_512x512.png"

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
  LV2URI https://surge-synthesizer.github.io/lv2/MoniqueMonosynth

  COPY_PLUGIN_AFTER_BUILD ${MONIQUE_COPY_PLUGIN_AFTER_BUILD}
  )


clap_juce_extensions_plugin(TARGET MoniqueMonosynth
  CLAP_ID "org.surge-synth-team.monique-monosynth"
  CLAP_FEATURES instrument synthesizer "mono synth" basses leads
  )


target_compile_definitions(${PROJECT_NAME} PUBLIC
  DROWAUDIO_USE_CURL=0
  JUCE_ALSA=1
  JUCE_JACK=1

  JUCE_DIRECTSOUND=1
  JUCE_WASAPI=1

  JUCE_USE_CURL=0
  JUCE_WEB_BROWSER=0
  JUCE_VST3_CAN_REPLACE_VST2=0

  JUCE_USE_FLAC=0
  JUCE_USE_LAME_AUDIO_FORMAT=0
  JUCE_USE_MP3AUDIOFORMAT=0
  JUCE_USE_OGGVORBIS=0
  JUCE_USE_WINDOWS_MEDIA_FORMAT=0
  JUCE_QUICKTIME=0

  # Note if you are building the code under MIT you need a license to use JUCE to set this to 0
  JUCE_DISPLAY_SPLASH_SCREEN=0
  JUCE_REPORT_APP_USAGE=0

  # Enables old code paths with MIDI IO handling
  IS_STANDALONE_WITH_OWN_AUDIO_MANAGER_AND_MIDI_HANDLING=0
  )

if(DEFINED ENV{ASIOSDK_DIR} OR BUILD_USING_MY_ASIO_LICENSE)
  if(BUILD_USING_MY_ASIO_LICENSE)
    message(STATUS "** BUILD USING OWN ASIO LICENSE **")
    message(STATUS "The resulting Monique standalone executable is not licensed for distribution!")
    message(STATUS "Fetching ASIO SDK...")

    set(ASIOSDK_DIR ${CMAKE_BINARY_DIR}/asio/asiosdk)
    add_custom_target(monique-get-local-asio)
    add_custom_command(
      TARGET monique-get-local-asio
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/asio
      COMMAND ${CMAKE_COMMAND} -D ASIO_SDK_DESTINATION=${CMAKE_BINARY_DIR}/asio -P cmake/get-asio.cmake
      )
    add_dependencies(${PROJECT_NAME} monique-get-local-asio)
  else()
    file(TO_CMAKE_PATH "$ENV{ASIOSDK_DIR}" ASIOSDK_DIR)
    message(STATUS "ASIO SDK found at ${ASIOSDK_DIR}")
    message(STATUS "The resulting Monique standalone executable is not licensed for distribution!")
  endif()

  target_compile_definitions(${PROJECT_NAME} PUBLIC JUCE_ASIO=1)
  target_include_directories(${PROJECT_NAME} PUBLIC ${ASIOSDK_DIR}/common)
  set(JUCE_ASIO_SUPPORT TRUE)
endif()

target_sources(${PROJECT_NAME}
  PRIVATE
    src/core/monique_core_Datastructures.cpp
    src/core/monique_core_Parameters.cpp
    src/core/monique_core_Processor.cpp
    src/core/monique_core_Synth.cpp
    src/ui/monique_ui_AmpPainter.cpp
    src/ui/monique_ui_Credits.cpp
    src/ui/monique_ui_DragPad.cpp
    src/ui/monique_ui_ENVPopup.cpp
    src/ui/monique_ui_GlobalSettings.cpp
    src/ui/monique_ui_Info.cpp
    src/ui/monique_ui_LookAndFeel.cpp
    src/ui/monique_ui_MFOPopup.cpp
    src/ui/monique_ui_MIDIIO.cpp
    src/ui/monique_ui_MainWindow.cpp
    src/ui/monique_ui_MidiLearnPopup.cpp
    src/ui/monique_ui_ModulationSlider.cpp
    src/ui/monique_ui_Morph.cpp
    src/ui/monique_ui_OptionPopup.cpp
    src/ui/monique_ui_Overlay.cpp
    src/ui/monique_ui_Playback.cpp
    src/ui/monique_ui_Refresher.cpp
    src/ui/monique_ui_SegmentedMeter.cpp
    src/core/mono_AudioDeviceManager.cpp
    )

juce_add_binary_data(MoniqueMonosynth_BinaryData
  SOURCES
    resources/files/A.zip
    resources/files/DARK.mcol
    "resources/files/FACTORTY DEFAULT.mlprog"
    resources/files/Themes.zip
    resources/fonts/SourceCodePro-Medium.otf
    resources/fonts/lato/Lato-Semibold.ttf
    resources/icon/monique_512x512.png
    resources/icon/monique_desktop_icon_1024x1024.png
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


if(APPLE)
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
    -Wlogical-op-parentheses
    -Wno-deprecated-declarations
    )
elseif(UNIX)
  message(STATUS "Turning off a legitimate compiler warning" )
  target_compile_options(${PROJECT_NAME} PUBLIC
    -Werror
    -Wno-deprecated-declarations
    # -ftime-trace
    )
else()
  # FIXME - I should be more careful about assuming windows == MSVC
  message(STATUS "Turning off a collection of legitimate compiler warnings" )
  target_compile_options(${PROJECT_NAME} PUBLIC
    /WX # Warnings are errors
    /W3 # and we show lots of them

    # This one needs fixing for sure
    /wd4996 # deprecation

    # these ones are probably ok in our context
    /wd4244 # double to float
    /wd4305 # float to double

    /permissive-  # allow the full spec
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  if(ASAN_ON)
    message(STATUS "Build with asan enabled")
    target_compile_options(${PROJECT_NAME} PUBLIC
      -fsanitize=address
      -fsanitize=undefined
      -fno-omit-frame-pointer
      )
    target_link_options(${PROJECT_NAME} PUBLIC
      -fsanitize=address
      -fsanitize=undefined
      )
  endif()
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
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/geninclude/version.cpp)

include(${CMAKE_SOURCE_DIR}/cmake/basic_installer.cmake)

# clang-format pipeline check
add_custom_target(code-quality-pipeline-checks)
set(CLANG_FORMAT_DIRS src/*)
set(CLANG_FORMAT_EXTS cpp h)
foreach(dir ${CLANG_FORMAT_DIRS})
  foreach(ext ${CLANG_FORMAT_EXTS})
    list(APPEND CLANG_FORMAT_GLOBS "':(glob)${dir}/**/*.${ext}'")
  endforeach()
endforeach()
add_custom_command(TARGET code-quality-pipeline-checks
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} -E echo About to check clang-format using clang-format-12
  COMMAND git ls-files -- ${CLANG_FORMAT_GLOBS} | xargs clang-format-12 --dry-run --Werror
  )
