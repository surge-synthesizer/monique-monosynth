#ifndef MONO_UI_REFRESHER_H_INCLUDED
#define MONO_UI_REFRESHER_H_INCLUDED

#include "App.h"

//==============================================================================
class MoniqueAudioProcessor;
class Monique_Ui_Refresher;
class UiLookAndFeel;
class MIDIControlHandler;
class MoniqueSynthData;
class Monique_Ui_Mainwindow;
class MoniqueSynthesiserVoice;

class Monique_Ui_Refreshable
{
  protected:
    // CLASSES NEEDED BY THE UI CLASSES
    Monique_Ui_Refresher *const ui_refresher;
    UiLookAndFeel *const look_and_feel;
    MIDIControlHandler *const midi_control_handler;

  public:
    MoniqueSynthData *const synth_data;

  protected:
    MoniqueSynthesiserVoice *const voice;

    Monique_Ui_Mainwindow *get_editor() noexcept;

  public:
    virtual void refresh() noexcept = 0;

  protected:
    Monique_Ui_Refreshable(Monique_Ui_Refresher *ui_refresher_) noexcept;
    ~Monique_Ui_Refreshable() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_Refreshable)
};

//==============================================================================
class Monique_Ui_Refresher : public Timer
{
  public:
    MoniqueAudioProcessor *const audio_processor;
    UiLookAndFeel *const look_and_feel;
    MIDIControlHandler *const midi_control_handler;
    MoniqueSynthData *const synth_data;
    MoniqueSynthesiserVoice *const voice;
    Monique_Ui_Mainwindow *editor; // WILL BE SET BY THE MAINWINDOW

    CriticalSection lock;
    Array<Monique_Ui_Refreshable *> refreshables;

    void timerCallback() override;
    void pause() noexcept { stopTimer(); }
    void go_on() noexcept { startTimer(UI_REFRESH_RATE); }

  private:
    //==========================================================================
    friend class Monique_Ui_Refreshable;
    void add(Monique_Ui_Refreshable *const) noexcept;
    void remove(Monique_Ui_Refreshable *const) noexcept;

  public:
    //==========================================================================
    void remove_all() noexcept;

  private:
    //==========================================================================
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy<Monique_Ui_Refresher>;

  public:
    Monique_Ui_Refresher(MoniqueAudioProcessor *audio_processor_,
                         UiLookAndFeel *const look_and_feel_,
                         MIDIControlHandler *const midi_control_handler_,
                         MoniqueSynthData *const synth_data_,
                         MoniqueSynthesiserVoice *const voice_) noexcept
        : audio_processor(audio_processor_), look_and_feel(look_and_feel_),
          midi_control_handler(midi_control_handler_), synth_data(synth_data_), voice(voice_),
          editor(nullptr)
    {
    }
    ~Monique_Ui_Refresher() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Monique_Ui_Refresher)
};

#endif
