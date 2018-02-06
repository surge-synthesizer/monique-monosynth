# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=x32
endif

ifeq ($(CONFIG),x32)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/x32
  JUCE_OUTDIR := ../LINUX

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m32
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG -DIS_STANDALONE -DPRINT_STACK_TRACE -DJUCER_LINUX_MAKE_7337D22A=1 -DJUCE_APP_VERSION=1.1 -DJUCE_APP_VERSION_HEX=0x10100 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../../JUCE-4.3.1/modules $(CPPFLAGS)
  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -Ofast -flto $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++11 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -ldl -lpthread -lrt  $(LDFLAGS)

  TARGET := Monique_32
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),x64)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/x64
  JUCE_OUTDIR := ../LINUX

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG -DIS_STANDALONE -DPRINT_STACK_TRACE -DJUCER_LINUX_MAKE_7337D22A=1 -DJUCE_APP_VERSION=1.1 -DJUCE_APP_VERSION_HEX=0x10100 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../../JUCE-4.3.1/modules $(CPPFLAGS)
  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -Ofast -flto $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++11 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -ldl -lpthread -lrt  $(LDFLAGS)

  TARGET := Monique_64
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/monique_core_Datastructures_d410a63f.o \
  $(JUCE_OBJDIR)/monique_core_Parameters_93b9539f.o \
  $(JUCE_OBJDIR)/monique_core_Processor_aa317247.o \
  $(JUCE_OBJDIR)/monique_core_Synth_27fdfbf1.o \
  $(JUCE_OBJDIR)/monique_ui_Activate_b66e153.o \
  $(JUCE_OBJDIR)/monique_ui_AmpPainter_161972c7.o \
  $(JUCE_OBJDIR)/monique_ui_Credits_1adb5dc4.o \
  $(JUCE_OBJDIR)/monique_ui_Demo_17925fc3.o \
  $(JUCE_OBJDIR)/monique_ui_DragPad_4a969d69.o \
  $(JUCE_OBJDIR)/monique_ui_ENVPopup_1cc5ce1f.o \
  $(JUCE_OBJDIR)/monique_ui_GlobalSettings_508d5f86.o \
  $(JUCE_OBJDIR)/monique_ui_LookAndFeel_cf505528.o \
  $(JUCE_OBJDIR)/monique_ui_MainWindow_f360d1a9.o \
  $(JUCE_OBJDIR)/monique_ui_MFOPopup_52b4b36.o \
  $(JUCE_OBJDIR)/monique_ui_MIDIIO_8b303507.o \
  $(JUCE_OBJDIR)/monique_ui_MidiLearnPopup_9b8abe9.o \
  $(JUCE_OBJDIR)/monique_ui_ModulationSlider_ba7639dd.o \
  $(JUCE_OBJDIR)/monique_ui_Morph_2d3a4c72.o \
  $(JUCE_OBJDIR)/monique_ui_OptionPopup_88264961.o \
  $(JUCE_OBJDIR)/monique_ui_Overlay_13a5227a.o \
  $(JUCE_OBJDIR)/monique_ui_Playback_a3680e7b.o \
  $(JUCE_OBJDIR)/monique_ui_Refresher_b7c42db2.o \
  $(JUCE_OBJDIR)/monique_ui_SegmentedMeter_4465f9b7.o \
  $(JUCE_OBJDIR)/mono_AudioDeviceManager_ed9e7cad.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o \
  $(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o \
  $(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking Monique
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning Monique
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping Monique
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_core_Datastructures_d410a63f.o: ../../../Source/monique_core_Datastructures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_core_Datastructures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_core_Parameters_93b9539f.o: ../../../Source/monique_core_Parameters.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_core_Parameters.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_core_Processor_aa317247.o: ../../../Source/monique_core_Processor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_core_Processor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_core_Synth_27fdfbf1.o: ../../../Source/monique_core_Synth.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_core_Synth.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_Activate_b66e153.o: ../../../Source/monique_ui_Activate.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_Activate.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_AmpPainter_161972c7.o: ../../../Source/monique_ui_AmpPainter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_AmpPainter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_Credits_1adb5dc4.o: ../../../Source/monique_ui_Credits.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_Credits.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_Demo_17925fc3.o: ../../../Source/monique_ui_Demo.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_Demo.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_DragPad_4a969d69.o: ../../../Source/monique_ui_DragPad.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_DragPad.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_ENVPopup_1cc5ce1f.o: ../../../Source/monique_ui_ENVPopup.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_ENVPopup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_GlobalSettings_508d5f86.o: ../../../Source/monique_ui_GlobalSettings.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_GlobalSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_LookAndFeel_cf505528.o: ../../../Source/monique_ui_LookAndFeel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_LookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_MainWindow_f360d1a9.o: ../../../Source/monique_ui_MainWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_MainWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_MFOPopup_52b4b36.o: ../../../Source/monique_ui_MFOPopup.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_MFOPopup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_MIDIIO_8b303507.o: ../../../Source/monique_ui_MIDIIO.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_MIDIIO.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_MidiLearnPopup_9b8abe9.o: ../../../Source/monique_ui_MidiLearnPopup.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_MidiLearnPopup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_ModulationSlider_ba7639dd.o: ../../../Source/monique_ui_ModulationSlider.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_ModulationSlider.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_Morph_2d3a4c72.o: ../../../Source/monique_ui_Morph.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_Morph.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_OptionPopup_88264961.o: ../../../Source/monique_ui_OptionPopup.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_OptionPopup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_Overlay_13a5227a.o: ../../../Source/monique_ui_Overlay.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_Overlay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_Playback_a3680e7b.o: ../../../Source/monique_ui_Playback.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_Playback.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_Refresher_b7c42db2.o: ../../../Source/monique_ui_Refresher.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_Refresher.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/monique_ui_SegmentedMeter_4465f9b7.o: ../../../Source/monique_ui_SegmentedMeter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling monique_ui_SegmentedMeter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/mono_AudioDeviceManager_ed9e7cad.o: ../../../Source/mono_AudioDeviceManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling mono_AudioDeviceManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
