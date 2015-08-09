//

//  juce_ios_Audio.h

//

//

//


#ifndef JuceDemo_juce_ios_Audio_h
#define JuceDemo_juce_ios_Audio_h

class iOSAudioIODevice  : public AudioIODevice {
public:

    iOSAudioIODevice (const String& deviceName);
    ~iOSAudioIODevice();

    StringArray getOutputChannelNames() override;
    StringArray getInputChannelNames() override;

    Array<double> getAvailableSampleRates() override;
    Array<int> getAvailableBufferSizes() override;

    int getDefaultBufferSize() override;
    
    String open (const BigInteger& inputChannelsWanted,
                 const BigInteger& outputChannelsWanted,
                 double targetSampleRate, int bufferSize) override;
    
    void close() override;
    bool isOpen() override;
    
    int getCurrentBufferSizeSamples() override;
    double getCurrentSampleRate() override;
    int getCurrentBitDepth() override;
    
    BigInteger getActiveOutputChannels() const override;
    BigInteger getActiveInputChannels() const override;

    int getOutputLatencyInSamples() override;
    int getInputLatencyInSamples() override;
    //int getLatency (AudioSessionPropertyID propID);

    void start (AudioIODeviceCallback* newCallback) override;
    void stop() override;

    bool isPlaying() override;
    String getLastError() override;

    bool setAudioPreprocessingEnabled (bool enable) override;

    void routingChanged (const NSNotification* notification);

private:

    //==================================================================================================
    
    void* wrapper; //Objective C class for recieving notifications from AVAudioSession
    AVAudioSession* avAudioSession; //the shared AVAudioSession
    NSError* err;

    CriticalSection callbackLock;
    Float64 sampleRate;
    int numInputChannels, numOutputChannels;
    int preferredBufferSize, actualBufferSize;
    bool isRunning;
    String lastError;

    AudioStreamBasicDescription format;
    AudioUnit audioUnit;
    UInt32 audioInputIsAvailable;
    AudioIODeviceCallback* callback;
    BigInteger activeOutputChans, activeInputChans;

    AudioSampleBuffer floatData;
    float* inputChannels[3];
    float* outputChannels[3];

    bool monoInputChannelNumber, monoOutputChannelNumber;

    void prepareFloatBuffers (int bufferSize);

    //==================================================================================================

    OSStatus process (AudioUnitRenderActionFlags* flags, const AudioTimeStamp* time,
                      const UInt32 numFrames, AudioBufferList* data);

    void updateDeviceInfo();

    void updateCurrentBufferSize();

    //==================================================================================================

    struct AudioSessionHolder {
        AudioSessionHolder() {
            //AudioSessionInitialize (0, 0, interruptionListenerCallback, this);
            //don't think this has to be done for AVAudioSession
        }

        static void interruptionListenerCallback (void* client, UInt32 interruptionType) {
            const Array <iOSAudioIODevice*>& activeDevices = static_cast <AudioSessionHolder*> (client)->activeDevices;

            for (int i = activeDevices.size(); --i >= 0;)
                activeDevices.getUnchecked(i)->interruptionListener (interruptionType);
        }

        Array <iOSAudioIODevice*> activeDevices;
    };

    static AudioSessionHolder& getSessionHolder() {
        static AudioSessionHolder audioSessionHolder;
        return audioSessionHolder;
    }
    
    void interruptionListener (const UInt32 interruptionType);

    //==================================================================================================

    static OSStatus processStatic (void* client, AudioUnitRenderActionFlags* flags, const AudioTimeStamp* time,
                                   UInt32 /*busNumber*/, UInt32 numFrames, AudioBufferList* data)
    {
        return static_cast<iOSAudioIODevice*> (client)->process (flags, time, numFrames, data);
    }

    // Routing changes have to be handled differently with AVAudioSession. However, I can't seem to find any reason that we actually need to respond to routing changes here, so haven't taken a stab at it.

    //    static void routingChangedStatic (void* client, AudioSessionPropertyID, UInt32 /*inDataSize*/, const void* propertyValue)
    //    {
    //        static_cast<iOSAudioIODevice*> (client)->routingChanged (propertyValue);
    //    }
    
    //==================================================================================================

    void resetFormat (const int numChannels) noexcept;

    bool createAudioUnit();

    // all of this is unnecessary with AVAudioSession as far as I can see:

    // If the routing is set to go through the receiver (i.e. the speaker, but quiet), this re-routes it
    // to make it loud. Needed because by default when using an input + output, the output is kept quiet.
    //static void fixAudioRouteIfSetToReceiver();

    //void fixAudioRouteIfSetToReceiver();

//    template <typename Type>
//    static OSStatus getSessionProperty (AudioSessionPropertyID propID, Type& result) noexcept
//    {
//        UInt32 valueSize = sizeof (result);
//        return AudioSessionGetProperty (propID, &valueSize, &result);
//    }

//    static bool setSessionUInt32Property  (AudioSessionPropertyID propID, UInt32  v) noexcept  { AudioSessionSetProperty (propID, sizeof (v), &v) == kAudioSessionNoError; }
//    static bool setSessionFloat32Property (AudioSessionPropertyID propID, Float32 v) noexcept  { AudioSessionSetProperty (propID, sizeof (v), &v) == kAudioSessionNoError; }
//    static bool setSessionFloat64Property (AudioSessionPropertyID propID, Float64 v) noexcept  { AudioSessionSetProperty (propID, sizeof (v), &v) == kAudioSessionNoError; }

    JUCE_DECLARE_NON_COPYABLE (iOSAudioIODevice)
};

#endif
