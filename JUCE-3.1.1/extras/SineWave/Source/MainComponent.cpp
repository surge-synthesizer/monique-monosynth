/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	sineWaveGenerator.setFrequency(200.0f);
	deviceManager.addAudioCallback(this);
	addAndMakeVisible (deviceSelector = new AudioDeviceSelectorComponent (deviceManager, 0, 0, 2, 2, false, false, true, false));
    setSize (500, 400);
}

MainContentComponent::~MainContentComponent()
{
	deviceSelector = nullptr;
}

void MainContentComponent::audioDeviceAboutToStart (AudioIODevice* device)
{
	Stk::setSampleRate(device->getCurrentSampleRate());
}

void MainContentComponent::audioDeviceStopped()
{
}

void MainContentComponent::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                                       float** outputChannelData, int numOutputChannels, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
		StkFloat data = sineWaveGenerator.tick();
        for (int chan = 0; chan < numOutputChannels; ++chan)
        {
			outputChannelData[chan][i] = (float)data;
        }
	}
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffffffff));
}

void MainContentComponent::resized()
{
	deviceSelector->setBounds (8, 8, getWidth() - 16, getHeight() - 16);
}
