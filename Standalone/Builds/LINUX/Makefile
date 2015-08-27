# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=x32
endif

ifeq ($(CONFIG),x32)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/x32
  OUTDIR := ../LINUX

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m32
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG" -D "MONIQUE_STANDALONE" -D "JUCER_LINUX_MAKE_7337D22A=1" -D "JUCE_APP_VERSION=0.5" -D "JUCE_APP_VERSION_HEX=0x500" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../JUCE-3.1.1/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Ofast -ffunction-sections  -fdata-sections -ffast-math -ftree-vectorize -flto
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -m64

  TARGET := Monique_32
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),x64)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/x64
  OUTDIR := ../LINUX

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG" -D "MONIQUE_STANDALONE" -D "JUCER_LINUX_MAKE_7337D22A=1" -D "JUCE_APP_VERSION=0.5" -D "JUCE_APP_VERSION_HEX=0x500" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../JUCE-3.1.1/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Ofast -ffunction-sections  -fdata-sections -ffast-math -ftree-vectorize -flto
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -m64

  TARGET := Monique_64
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/monique_AppInstanceStore_ee3002c0.o \
  $(OBJDIR)/monique_ui_ADRDesigner_ad1be764.o \
  $(OBJDIR)/monique_ui_AmpPainter_161972c7.o \
  $(OBJDIR)/monique_ui_DragPad_4a969d69.o \
  $(OBJDIR)/monique_ui_GlobalSettings_508d5f86.o \
  $(OBJDIR)/monique_ui_MIDIIO_8b303507.o \
  $(OBJDIR)/monique_ui_ModulationSlider_ba7639dd.o \
  $(OBJDIR)/monique_ui_Refresher_b7c42db2.o \
  $(OBJDIR)/monique_ui_SegmentedMeter_4465f9b7.o \
  $(OBJDIR)/mono_AudioDeviceManager_ed9e7cad.o \
  $(OBJDIR)/mono_Parameters_6e808cd6.o \
  $(OBJDIR)/PluginProcessor_319d19ef.o \
  $(OBJDIR)/Synth_8129adcc.o \
  $(OBJDIR)/SynthData_df69b916.o \
  $(OBJDIR)/UiEditorMorph_b9084a77.o \
  $(OBJDIR)/UiEditorSynthLite_e00d3d79.o \
  $(OBJDIR)/UiEditorSynthLitePopup_a4990cfd.o \
  $(OBJDIR)/UiLookAndFeel_8efd25da.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_49d7f937.o \
  $(OBJDIR)/juce_audio_devices_b969dc35.o \
  $(OBJDIR)/juce_audio_formats_e7eb25d5.o \
  $(OBJDIR)/juce_audio_processors_f677abef.o \
  $(OBJDIR)/juce_audio_utils_5ab2d675.o \
  $(OBJDIR)/juce_core_a2cfd019.o \
  $(OBJDIR)/juce_data_structures_858cbb15.o \
  $(OBJDIR)/juce_events_355a880d.o \
  $(OBJDIR)/juce_graphics_453298f1.o \
  $(OBJDIR)/juce_gui_basics_c4ee1fed.o \
  $(OBJDIR)/juce_gui_extra_e64168b5.o \

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

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_AppInstanceStore_ee3002c0.o: ../../../Source/monique_AppInstanceStore.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_AppInstanceStore.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_ADRDesigner_ad1be764.o: ../../../Source/monique_ui_ADRDesigner.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_ADRDesigner.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_AmpPainter_161972c7.o: ../../../Source/monique_ui_AmpPainter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_AmpPainter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_DragPad_4a969d69.o: ../../../Source/monique_ui_DragPad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_DragPad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_GlobalSettings_508d5f86.o: ../../../Source/monique_ui_GlobalSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_GlobalSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_MIDIIO_8b303507.o: ../../../Source/monique_ui_MIDIIO.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_MIDIIO.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/monique_ui_ModulationSlider_ba7639dd.o: ../../../Source/monique_ui_ModulationSlider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling monique_ui_ModulationSlider.cpp"
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

$(OBJDIR)/mono_Parameters_6e808cd6.o: ../../../Source/mono_Parameters.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling mono_Parameters.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginProcessor_319d19ef.o: ../../../Source/PluginProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Synth_8129adcc.o: ../../../Source/Synth.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Synth.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SynthData_df69b916.o: ../../../Source/SynthData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SynthData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UiEditorMorph_b9084a77.o: ../../../Source/UiEditorMorph.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UiEditorMorph.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UiEditorSynthLite_e00d3d79.o: ../../../Source/UiEditorSynthLite.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UiEditorSynthLite.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UiEditorSynthLitePopup_a4990cfd.o: ../../../Source/UiEditorSynthLitePopup.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UiEditorSynthLitePopup.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UiLookAndFeel_8efd25da.o: ../../../Source/UiLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UiLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_49d7f937.o: ../../../JUCE-3.1.1/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_b969dc35.o: ../../../JUCE-3.1.1/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_e7eb25d5.o: ../../../JUCE-3.1.1/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_f677abef.o: ../../../JUCE-3.1.1/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_5ab2d675.o: ../../../JUCE-3.1.1/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_a2cfd019.o: ../../../JUCE-3.1.1/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_858cbb15.o: ../../../JUCE-3.1.1/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_355a880d.o: ../../../JUCE-3.1.1/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_453298f1.o: ../../../JUCE-3.1.1/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_c4ee1fed.o: ../../../JUCE-3.1.1/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_e64168b5.o: ../../../JUCE-3.1.1/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
