/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_9D7C2FF8__
#define __MAINCOMPONENT_H_9D7C2FF8__

#include "../JuceLibraryCode/JuceHeader.h"

using namespace stk;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public AudioIODeviceCallback
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

	void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels, int numSamples);

    void paint (Graphics&);
    void resized();

private:
    //==============================================================================
	AudioDeviceManager deviceManager;
	ScopedPointer<AudioDeviceSelectorComponent> deviceSelector;
	SineWave sineWaveGenerator;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // __MAINCOMPONENT_H_9D7C2FF8__
