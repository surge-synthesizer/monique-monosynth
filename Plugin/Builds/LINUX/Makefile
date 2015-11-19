# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=x64
endif

ifeq ($(CONFIG),x64)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/x64
  OUTDIR := ../LINUX

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "IS_PLUGIN" -D "JUCER_LINUX_MAKE_7337D22A=1" -D "JUCE_APP_VERSION=0.9.8" -D "JUCE_APP_VERSION_HEX=0x908" -I /usr/include -I /usr/include/freetype2 -I ../../../vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JUCE-4.0.1/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -Ofast -flto
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -Wl,--no-undefined -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := Monique_64x.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),x32)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/x32
  OUTDIR := ../LINUX

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m32
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "IS_PLUGIN" -D "JUCER_LINUX_MAKE_7337D22A=1" -D "JUCE_APP_VERSION=0.9.8" -D "JUCE_APP_VERSION_HEX=0x908" -I /usr/include -I /usr/include/freetype2 -I ../../../vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JUCE-4.0.1/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -Ofast -flto
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -Wl,--no-undefined -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := Monique_32x.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/monique_ui_MFOPopup_52b4b36.o \
  $(OBJDIR)/monique_core_Datastructures_d410a63f.o \
  $(OBJDIR)/monique_core_Parameters_93b9539f.o \
  $(OBJDIR)/monique_core_Processor_aa317247.o \
  $(OBJDIR)/monique_core_Synth_27fdfbf1.o \
  $(OBJDIR)/monique_ui_AmpPainter_161972c7.o \
  $(OBJDIR)/monique_ui_DragPad_4a969d69.o \
  $(OBJDIR)/monique_ui_ENVPopup_1cc5ce1f.o \
  $(OBJDIR)/monique_ui_GlobalSettings_508d5f86.o \
  $(OBJDIR)/monique_ui_LookAndFeel_cf505528.o \
  $(OBJDIR)/monique_ui_MainWindow_f360d1a9.o \
  $(OBJDIR)/monique_ui_MIDIIO_8b303507.o \
  $(OBJDIR)/monique_ui_MidiLearnPopup_9b8abe9.o \
  $(OBJDIR)/monique_ui_ModulationSlider_ba7639dd.o \
  $(OBJDIR)/monique_ui_Morph_2d3a4c72.o \
  $(OBJDIR)/monique_ui_OptionPopup_88264961.o \
  $(OBJDIR)/monique_ui_Refresher_b7c42db2.o \
  $(OBJDIR)/monique_ui_SegmentedMeter_4465f9b7.o \
  $(OBJDIR)/mono_AudioDeviceManager_ed9e7cad.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_93878cf7.o \
  $(OBJDIR)/juce_audio_devices_55837ff5.o \
  $(OBJDIR)/juce_audio_formats_8404c995.o \
  $(OBJDIR)/juce_audio_processors_a0377faf.o \
  $(OBJDIR)/juce_audio_utils_9eb45a35.o \
  $(OBJDIR)/juce_core_4f9ee3d9.o \
  $(OBJDIR)/juce_cryptography_950c9033.o \
  $(OBJDIR)/juce_data_structures_70ae7ed5.o \
  $(OBJDIR)/juce_events_f8f9bbcd.o \
  $(OBJDIR)/juce_graphics_fa91ecb1.o \
  $(OBJDIR)/juce_gui_basics_4efd93ad.o \
  $(OBJDIR)/juce_gui_extra_c11acc75.o \
  $(OBJDIR)/juce_PluginUtilities_caf33901.o \
  $(OBJDIR)/juce_VST_Wrapper_8a92a50a.o \
  $(OBJDIR)/juce_VST3_Wrapper_3a17c3c8.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking Monique
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning Monique
	@$(CLEANCMD)

strip:
	@echo Stripping Monique
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/monique_ui_MFOPopup_52b4b36.o: ../../../Source/monique_ui_MFOPopup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_MFOPopup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_core_Datastructures_d410a63f.o: ../../../Source/monique_core_Datastructures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_core_Datastructures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_core_Parameters_93b9539f.o: ../../../Source/monique_core_Parameters.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_core_Parameters.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_core_Processor_aa317247.o: ../../../Source/monique_core_Processor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_core_Processor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_core_Synth_27fdfbf1.o: ../../../Source/monique_core_Synth.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_core_Synth.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_AmpPainter_161972c7.o: ../../../Source/monique_ui_AmpPainter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_AmpPainter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_DragPad_4a969d69.o: ../../../Source/monique_ui_DragPad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_DragPad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_ENVPopup_1cc5ce1f.o: ../../../Source/monique_ui_ENVPopup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_ENVPopup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_GlobalSettings_508d5f86.o: ../../../Source/monique_ui_GlobalSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_GlobalSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_LookAndFeel_cf505528.o: ../../../Source/monique_ui_LookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_LookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_MainWindow_f360d1a9.o: ../../../Source/monique_ui_MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_MIDIIO_8b303507.o: ../../../Source/monique_ui_MIDIIO.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_MIDIIO.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_MidiLearnPopup_9b8abe9.o: ../../../Source/monique_ui_MidiLearnPopup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_MidiLearnPopup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_ModulationSlider_ba7639dd.o: ../../../Source/monique_ui_ModulationSlider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_ModulationSlider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_Morph_2d3a4c72.o: ../../../Source/monique_ui_Morph.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_Morph.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_OptionPopup_88264961.o: ../../../Source/monique_ui_OptionPopup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_OptionPopup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_Refresher_b7c42db2.o: ../../../Source/monique_ui_Refresher.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_Refresher.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_SegmentedMeter_4465f9b7.o: ../../../Source/monique_ui_SegmentedMeter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_SegmentedMeter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/mono_AudioDeviceManager_ed9e7cad.o: ../../../Source/mono_AudioDeviceManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling mono_AudioDeviceManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_93878cf7.o: ../../../JUCE-4.0.1/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_55837ff5.o: ../../../JUCE-4.0.1/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_8404c995.o: ../../../JUCE-4.0.1/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_a0377faf.o: ../../../JUCE-4.0.1/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_9eb45a35.o: ../../../JUCE-4.0.1/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_4f9ee3d9.o: ../../../JUCE-4.0.1/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_950c9033.o: ../../../JUCE-4.0.1/modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_70ae7ed5.o: ../../../JUCE-4.0.1/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_f8f9bbcd.o: ../../../JUCE-4.0.1/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_fa91ecb1.o: ../../../JUCE-4.0.1/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_4efd93ad.o: ../../../JUCE-4.0.1/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_c11acc75.o: ../../../JUCE-4.0.1/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_caf33901.o: ../../../JUCE-4.0.1/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_8a92a50a.o: ../../../JUCE-4.0.1/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_3a17c3c8.o: ../../../JUCE-4.0.1/modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST3_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
