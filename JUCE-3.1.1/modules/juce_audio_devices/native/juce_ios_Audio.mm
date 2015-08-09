#define AUDIOBUS

#include "juce_ios_Audio.h"

#import <AVFoundation/AVAudioSession.h>
#import <AudioToolbox/AudioToolbox.h>


} // juce namespace


#ifdef AUDIOBUS

#import "Audiobus.h"
//#include "AudiobusStatus.h"

#endif


// Objective C class with some methods for working with AVAudioSession and Audiobus.

// This holds a pointer to its parent so we can send notifications to it.


@interface Wrapper : NSObject
{
    juce::iOSAudioIODevice* owner;

#ifdef AUDIOBUS
    ABSenderPort *audiobusOutput;
    ABReceiverPort *audiobusInput;
#endif
}


#ifdef AUDIOBUS
@property (readonly) ABSenderPort* audiobusOutput;
@property (strong, nonatomic) ABAudiobusController* audiobusController;
@property (nonatomic, strong) ABReceiverPort *receiverPort;
#endif

- (void)registerForRouteChangeNotification;

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context;

- (void)routeChange:(NSNotification*)notification;

- (void)activateAudiobus:(AudioUnit)outputUnit;
- (void)activateAudiobusInput:()inputUnit;
@end


//-------------

static void * kAudiobusRunningOrConnectedChanged = &kAudiobusRunningOrConnectedChanged;

@implementation Wrapper

#ifdef AUDIOBUS
@synthesize audiobusOutput;
@synthesize audiobusController;
#endif


- (id) initWithOwner: (juce::iOSAudioIODevice*) owner_ {
    if ((self = [super init]) != nil) {
        owner = owner_;
    };
    
    return self;
}

- (void)dealloc {
#ifdef AUDIOBUS
    [audiobusController removeObserver:self forKeyPath:@"connected"];
    [audiobusController removeObserver:self forKeyPath:@"audiobusAppRunning"];
#endif

    [super dealloc];
}

- (void) registerForRouteChangeNotification {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(routeChange:)
                                                 name:AVAudioSessionRouteChangeNotification
                                               object:nil];
}

- (void)routeChange:(NSNotification*)notification {
    // It doesn't appear Juce needs to do anything with routing changes, so I haven't bothered with this yet:
    
    //owner->routingChanged (notification);
}

-(void)observeValueForKeyPath:(NSString *)keyPath
                     ofObject:(id)object
                       change:(NSDictionary *)change
                      context:(void *)context {
#ifdef AUDIOBUS
    if ( context == kAudiobusRunningOrConnectedChanged ) {
        //I created this AudiobusStatus singleton so that I can easily check audiobus' status from elsewhere in the program. I need to check the Audiobus connection status when going into the background for example. Could probably find a more elegant way to do this.
        
        // AudiobusStatus* statusObject = AudiobusStatus::getInstance();
        // statusObject->setConnected (audiobusController.audiobusConnected);
        // statusObject->setRunning (audiobusController.audiobusAppRunning);
        
        //just testing
        if (!audiobusController.audiobusAppRunning) {
            // Audiobus has quit. Time to sleep.
            NSLog(@"Audiobus app has closed");
        }

        if(!audiobusController.audiobusConnected) {
            NSLog(@"B-Step disconnected from Audiobus");
        }
    }
    else {
        [super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
    }
#endif
}

- (void)activateAudiobus:(AudioUnit)outputUnit; {
#ifdef AUDIOBUS
    NSLog(@"B-Step Output activateAudiobus");
#ifndef DEMO // FULL
    // AUDIO BUS API KEY
    self.audiobusController = [[ABAudiobusController alloc] initWithApiKey:@"MCoqKkItU3RlcCoqKkItU3RlcC5hdWRpb2J1czovLw==:glY0ZTKGjoR71UtAifQrTIvhBK1cyQw5ju/67YlO2DFvFV1yfQZVLwsHStdVlw8oB8Yhg2GX5lbFWQ7KZqX91RsC76DoWCMq/BqnpeaXG1fIWmNTeK1XJRTb3D6Et1l6"];
#else // DEMO
    self.audiobusController = [[ABAudiobusController alloc] initWithApiKey:@"MCoqKkItU3RlcCBUcmlhbCoqKkItU3RlcC1UcmlhbC5hdWRpb2J1czovLw==:GK+79/lOJzVr1PIUeGnPni7hysPbPTUFRG5kjCZ/zqDaGuu1KUpRqxj6oQYumfW/WV8fqpw3GUqG0ajL8m06zcryrDsv9EgNKlE/gy62PsEPeUOSgZMX4UY37/9M8O3Z"];
#endif
    
    //choose where you want the Audiobus navigation widget to show up
    self.audiobusController.connectionPanelPosition = ABConnectionPanelPositionBottom;
#ifndef DEMO // FULL
    // port information needs to match the information entered into the .plist (see audiobus integration guide)
    ABSenderPort *sender = [[ABSenderPort alloc] initWithName:@"Monoplugs: B-Step"
                                                        title:NSLocalizedString(@"B-Step", @"")
                                    audioComponentDescription:(AudioComponentDescription) {
                                        .componentType = kAudioUnitType_RemoteGenerator,
                                        .componentSubType = 'sequ', // Note single quotes
                                        .componentManufacturer = 'MoPl' } //
                                        audioUnit:outputUnit];
#else // DEMO
    ABSenderPort *sender = [[ABSenderPort alloc] initWithName:@"Monoplugs: B-Step Trial"
                                                        title:NSLocalizedString(@"B-Step", @"")
                                    audioComponentDescription:(AudioComponentDescription) {
                                        .componentType = kAudioUnitType_RemoteGenerator,
                                        .componentSubType = 'seqt', // Note single quotes
                                        .componentManufacturer = 'MoPl' } //
                                                    audioUnit:outputUnit];
#endif

    
    [audiobusController addSenderPort:sender];
    
    // would create filter or input ports here if I needed them
    // Watch the audiobusAppRunning and connected properties
    [audiobusController addObserver:self
                         forKeyPath:@"connected"
                            options:0
                            context:kAudiobusRunningOrConnectedChanged];

    [audiobusController addObserver:self
                         forKeyPath:@"audiobusAppRunning"
                            options:0
                            context:kAudiobusRunningOrConnectedChanged];
#endif
}
- (void)activateAudiobusInput:()inputUnit; {
#ifdef AUDIOBUS
    NSLog(@"B-Step Input activateAudiobus");
    self.receiverPort = [[ABReceiverPort alloc] initWithName:@"B-Step Audio Input"
                                                       title:NSLocalizedString(@"B-Step", @"")];
    [audiobusController addReceiverPort:_receiverPort];
    
    AudioStreamBasicDescription audioDescription = {
        .mFormatID          = kAudioFormatLinearPCM,
        .mFormatFlags       = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved,
        .mChannelsPerFrame  = 2,
        .mBytesPerPacket    = sizeof(float),
        .mFramesPerPacket   = 1,
        .mBytesPerFrame     = sizeof(float),
        .mBitsPerChannel    = 8 * sizeof(float),
        .mSampleRate        = 44100.0
    };
    _receiverPort.clientFormat = audioDescription;
#endif
}
@end


namespace juce {
    iOSAudioIODevice::iOSAudioIODevice (const String& deviceName)
    :
    AudioIODevice (deviceName, String("iOS Audio")),
    actualBufferSize (0),
    isRunning (false),
    audioUnit (0),
    callback (nullptr),
    floatData (1, 2)
{
    Wrapper* newWrapper = [[Wrapper alloc] initWithOwner: this];
    [newWrapper retain];
    wrapper = newWrapper;

    avAudioSession = [AVAudioSession sharedInstance];

    getSessionHolder().activeDevices.add (this);


    // TODO set on open?
    numInputChannels = 2;
    numOutputChannels = 2;

    preferredBufferSize = 0;

    updateDeviceInfo();
}

iOSAudioIODevice::~iOSAudioIODevice() {
    getSessionHolder().activeDevices.removeFirstMatchingValue (this);
    close();
    [avAudioSession setActive: NO error:  &err];
    std::cout << "killed iOSAudioIODevice"<< std::endl;
}

StringArray iOSAudioIODevice::getOutputChannelNames() {
    StringArray s;
    s.add ("Left");
    s.add ("Right");
    
    return s;
}

StringArray iOSAudioIODevice::getInputChannelNames() {
    StringArray s;
    
    if (audioInputIsAvailable) {
        s.add ("Left");
        s.add ("Right");
    }
    
    return s;
}

Array<double> iOSAudioIODevice::getAvailableSampleRates() {
    // can't find a good way to actually ask the device for which of these it supports..
    static const double rates[] = { 8000.0, 16000.0, 22050.0, 32000.0, 44100.0, 48000.0 };
    return Array<double> (rates, numElementsInArray (rates));
}


Array<int> iOSAudioIODevice::getAvailableBufferSizes() {
    Array<int> r;
    for (int i = 6; i < 12; ++i)
        r.add (1 << i);
    
    return r;
}

int iOSAudioIODevice::getDefaultBufferSize() { return 1024; }

String iOSAudioIODevice::open (const BigInteger& inputChannelsWanted,
                               const BigInteger& outputChannelsWanted,
                               double targetSampleRate, int bufferSize) {
    
    close();
    lastError.clear();

    preferredBufferSize = (bufferSize <= 0) ? getDefaultBufferSize() : bufferSize;
    
    //  xxx set up channel mapping
    activeOutputChans = outputChannelsWanted;
    activeOutputChans.setRange (2, activeOutputChans.getHighestBit(), false);
    numOutputChannels = activeOutputChans.countNumberOfSetBits();
    monoOutputChannelNumber = activeOutputChans.findNextSetBit (0);
    activeInputChans = inputChannelsWanted;
    activeInputChans.setRange (2, activeInputChans.getHighestBit(), false);
    numInputChannels = activeInputChans.countNumberOfSetBits();
    monoInputChannelNumber = activeInputChans.findNextSetBit (0);

    //set the audio session category before making the session active
    if (numInputChannels > 0 && audioInputIsAvailable) {
        //[avAudioSession setCategory: AVAudioSessionCategoryPlayAndRecord
        //                withOptions: AVAudioSessionCategoryOptionMixWithOthers | AVAudioSessionCategoryOptionDefaultToSpeaker |AVAudioSessionCategoryOptionAllowBluetooth
        //                      error:  &err];
    }
    else{
        [avAudioSession setCategory: AVAudioSessionCategoryPlayback
                        withOptions: AVAudioSessionCategoryOptionMixWithOthers
                              error:  &err];
        
        [avAudioSession setActive: YES error:  &err];
        
        [(id)wrapper registerForRouteChangeNotification];
        
        [avAudioSession setPreferredSampleRate:targetSampleRate error:&err];
        
        updateDeviceInfo();
        
        [avAudioSession setPreferredIOBufferDuration: preferredBufferSize/sampleRate error: &err];
        updateCurrentBufferSize();
        prepareFloatBuffers (actualBufferSize);
        
        isRunning = true;
        routingChanged (nullptr);  // creates and starts the AU
        
        lastError = audioUnit != 0 ? "" : "Couldn't open the device";
    }

    return lastError;
}


void iOSAudioIODevice::close() {
    if (isRunning){
        isRunning = false;
        
        //setSessionUInt32Property (kAudioSessionProperty_AudioCategory, kAudioSessionCategory_MediaPlayback);
        
        [avAudioSession setCategory: AVAudioSessionCategoryPlayback
                        withOptions: AVAudioSessionCategoryOptionMixWithOthers
                              error:  &err];
        
        //AudioSessionRemovePropertyListenerWithUserData (kAudioSessionProperty_AudioRouteChange, routingChangedStatic, this);
        
        [(id)wrapper release];
        
        if (audioUnit != 0) {
            AudioComponentInstanceDispose (audioUnit);
            audioUnit = 0;
        }
        
        //AudioSessionSetActive (false);
        
        [avAudioSession setActive: NO error:  &err];
    }
}

bool iOSAudioIODevice::isOpen()                       { return isRunning; }
int iOSAudioIODevice::getCurrentBufferSizeSamples()   { return actualBufferSize; }
double iOSAudioIODevice::getCurrentSampleRate()       { return sampleRate; }
int iOSAudioIODevice::getCurrentBitDepth()            { return 16; }

BigInteger iOSAudioIODevice::getActiveOutputChannels() const    { return activeOutputChans; }
BigInteger iOSAudioIODevice::getActiveInputChannels() const     { return activeInputChans; }

int iOSAudioIODevice::getOutputLatencyInSamples() {
    //return getLatency (kAudioSessionProperty_CurrentHardwareOutputLatency);
    
    double latency = avAudioSession.outputLatency;
    
    return roundToInt (latency * getCurrentSampleRate());
}

int iOSAudioIODevice::getInputLatencyInSamples() {
    //return getLatency (kAudioSessionProperty_CurrentHardwareInputLatency);
    
    double latency = avAudioSession.inputLatency;
    
    return roundToInt (latency * getCurrentSampleRate());
}

//int iOSAudioIODevice::getLatency (AudioSessionPropertyID propID)
//{
//    Float32 latency = 0;
//    getSessionProperty (propID, latency);
//    return roundToInt (latency * getCurrentSampleRate());
//}

void iOSAudioIODevice::start (AudioIODeviceCallback* newCallback) {
    if (isRunning && callback != newCallback) {
        if (newCallback != nullptr)
            newCallback->audioDeviceAboutToStart (this);
        
        const ScopedLock sl (callbackLock);
        callback = newCallback;
    }
}

void iOSAudioIODevice::stop() {
    if (isRunning) {
        AudioIODeviceCallback* lastCallback;
        
        {
            const ScopedLock sl (callbackLock);
            lastCallback = callback;
            callback = nullptr;
        }
        
        if (lastCallback != nullptr)
            lastCallback->audioDeviceStopped();
    }
}


bool iOSAudioIODevice::isPlaying()            { return isRunning && callback != nullptr; }

String iOSAudioIODevice::getLastError()       { return lastError; }
    
bool iOSAudioIODevice::setAudioPreprocessingEnabled (bool enable) {
    //    return setSessionUInt32Property (kAudioSessionProperty_Mode, enable ? kAudioSessionMode_Default
    //                                 : kAudioSessionMode_Measurement);
    
    return [avAudioSession setMode: enable ? AVAudioSessionModeDefault : AVAudioSessionModeMeasurement
                             error:  &err];
}

void iOSAudioIODevice::routingChanged (const NSNotification* notification) {
    if (! isRunning)
        return;

    if (notification != nullptr) {
        //        CFDictionaryRef routeChangeDictionary = (CFDictionaryRef) propertyValue;
        //        CFNumberRef routeChangeReasonRef = (CFNumberRef) CFDictionaryGetValue (routeChangeDictionary,
        //                                                                                CFSTR (kAudioSession_AudioRouteChangeKey_Reason));
        //
        //        SInt32 routeChangeReason;
        //        CFNumberGetValue (routeChangeReasonRef, kCFNumberSInt32Type, &routeChangeReason);
        //
        //        if (routeChangeReason == kAudioSessionRouteChangeReason_OldDeviceUnavailable)
        //        {
        //            const ScopedLock sl (callbackLock);
        //
        //            if (callback != nullptr)
        //                callback->audioDeviceError ("Old device unavailable");
        //        }
        //again, not doing anything here, but if you wanted to:
        
        NSDictionary *routeChangeDict = notification.userInfo;

        NSInteger routeChangeReason = [[routeChangeDict valueForKey:AVAudioSessionRouteChangeReasonKey] integerValue];

        switch (routeChangeReason) {
            case AVAudioSessionRouteChangeReasonUnknown:
                NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonUnknown");
                break;
            case AVAudioSessionRouteChangeReasonNewDeviceAvailable:
                // a headset was added or removed
                NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonNewDeviceAvailable");
                break;
            case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
                // a headset was added or removed
                NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonOldDeviceUnavailable");
                break;
            case AVAudioSessionRouteChangeReasonCategoryChange:
                // called at start - also when other audio wants to play
                NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonCategoryChange");//AVAudioSessionRouteChangeReasonCategoryChange
                break;
            case AVAudioSessionRouteChangeReasonOverride:
                NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonOverride");
                break;
            case AVAudioSessionRouteChangeReasonWakeFromSleep:
                NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonWakeFromSleep");
                break;
            case AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory:
                NSLog(@"routeChangeReason : AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory");
                break;
            default:
                break;
        }
        
        if (routeChangeReason == AVAudioSessionRouteChangeReasonOldDeviceUnavailable) {
            const ScopedLock sl (callbackLock);
            
            if (callback != nullptr)
                callback->audioDeviceError ("Old device unavailable");
        }
    }
    
    updateDeviceInfo();
    
    // TODO we create a audio unit or a reciver port
    createAudioUnit();

    //AudioSessionSetActive (true);
    [avAudioSession setActive: YES error:&err];
    
    if (audioUnit != 0) {
        UInt32 formatSize = sizeof (format);
        AudioUnitGetProperty (audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &format, &formatSize);
        
        updateCurrentBufferSize();
        AudioOutputUnitStart (audioUnit);
    }
}

//private functions
//---------------------

void iOSAudioIODevice::prepareFloatBuffers (int bufferSize) {
    if (numInputChannels + numOutputChannels > 0) {
        floatData.setSize (numInputChannels + numOutputChannels, bufferSize);
        zeromem (inputChannels, sizeof (inputChannels));
        zeromem (outputChannels, sizeof (outputChannels));
        
        for (int i = 0; i < numInputChannels; ++i)
            inputChannels[i] = floatData.getWritePointer (i);

        for (int i = 0; i < numOutputChannels; ++i)
            outputChannels[i] = floatData.getWritePointer (i + numInputChannels);
    }
}

//==================================================================================================

OSStatus iOSAudioIODevice::process (AudioUnitRenderActionFlags* flags, const AudioTimeStamp* time, const UInt32 numFrames, AudioBufferList* data) {
    OSStatus err = noErr;
    
    if (audioInputIsAvailable && numInputChannels > 0)
        err = AudioUnitRender (audioUnit, flags, time, 1, numFrames, data);

    const ScopedLock sl (callbackLock);

    if (callback != nullptr){
        if ((int) numFrames > floatData.getNumSamples())
            prepareFloatBuffers ((int) numFrames);

        if (audioInputIsAvailable && numInputChannels > 0) {
            short* shortData = (short*) data->mBuffers[0].mData;
            
            if (numInputChannels >= 2) {
                for (UInt32 i = 0; i < numFrames; ++i) {
                    inputChannels[0][i] = *shortData++ * (1.0f / 32768.0f);
                    inputChannels[1][i] = *shortData++ * (1.0f / 32768.0f);
                }
            }
            else {
                if (monoInputChannelNumber > 0)
                    ++shortData;
                
                for (UInt32 i = 0; i < numFrames; ++i) {
                    inputChannels[0][i] = *shortData++ * (1.0f / 32768.0f);
                    ++shortData;
                }
            }
        }
        else {
            for (int i = numInputChannels; --i >= 0;)
                zeromem (inputChannels[i], sizeof (float) * numFrames);
        }
        
        callback->audioDeviceIOCallback ((const float**) inputChannels, numInputChannels, outputChannels, numOutputChannels, (int) numFrames);

        short* shortData = (short*) data->mBuffers[0].mData;
        int n = 0;

        if (numOutputChannels >= 2) {
            for (UInt32 i = 0; i < numFrames; ++i) {
                shortData [n++] = (short) (outputChannels[0][i] * 32767.0f);
                shortData [n++] = (short) (outputChannels[1][i] * 32767.0f);
            }
        }
        else if (numOutputChannels == 1) {
            for (UInt32 i = 0; i < numFrames; ++i) {
                const short s = (short) (outputChannels[monoOutputChannelNumber][i] * 32767.0f);
                shortData [n++] = s;
                shortData [n++] = s;
            }
        }
        else {
            zeromem (data->mBuffers[0].mData, 2 * sizeof (short) * numFrames);
        }
    }
    else {
        zeromem (data->mBuffers[0].mData, 2 * sizeof (short) * numFrames);
    }

    return err;
}


void iOSAudioIODevice::updateDeviceInfo() {
    sampleRate = avAudioSession.sampleRate;
    audioInputIsAvailable = avAudioSession.inputAvailable;
}


void iOSAudioIODevice::updateCurrentBufferSize() {
    Float32 bufferDuration = sampleRate > 0 ? (Float32) (preferredBufferSize / sampleRate) : 0.0f;
    bufferDuration = avAudioSession.IOBufferDuration;
    actualBufferSize = (int) (sampleRate * bufferDuration + 0.5);
}

//==================================================================================================

void  iOSAudioIODevice::interruptionListener (const UInt32 interruptionType) {
    if (interruptionType == kAudioSessionBeginInterruption) {
        isRunning = false;
        AudioOutputUnitStop (audioUnit);
        //AudioSessionSetActive (false);
        [avAudioSession setActive: NO error:&err];

        const ScopedLock sl (callbackLock);

        if (callback != nullptr)
            callback->audioDeviceError ("iOS audio session interruption");
    }

    if (interruptionType == kAudioSessionEndInterruption) {
        isRunning = true;
        //AudioSessionSetActive (true);
        [avAudioSession setActive: YES error:&err];

        AudioOutputUnitStart (audioUnit);

        const ScopedLock sl (callbackLock);

        if (callback != nullptr)
            callback->audioDeviceError ("iOS audio session resumed");
    }
}

//==================================================================================================

void iOSAudioIODevice::resetFormat (const int numChannels) noexcept {
    zerostruct (format);
    format.mFormatID = kAudioFormatLinearPCM;
    format.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked | kAudioFormatFlagsNativeEndian;
    format.mBitsPerChannel = 8 * sizeof (short);
    format.mChannelsPerFrame = (UInt32) numChannels;
    format.mFramesPerPacket = 1;
    format.mBytesPerFrame = format.mBytesPerPacket = (UInt32) numChannels * sizeof (short);
}

bool iOSAudioIODevice::createAudioUnit() {
    if (audioUnit != 0) {
        AudioComponentInstanceDispose (audioUnit);
        audioUnit = 0;
    }

    resetFormat (2);

    AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;

    AudioComponent comp = AudioComponentFindNext (0, &desc);
    AudioComponentInstanceNew (comp, &audioUnit);

    if (audioUnit == 0)
        return false;
    
    if (numInputChannels > 0) {
        const UInt32 one = 1;
        AudioUnitSetProperty (audioUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, 1, &one, sizeof (one));
    }
    
    {
        AudioChannelLayout layout;
        layout.mChannelBitmap = 0;
        layout.mNumberChannelDescriptions = 0;
        layout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
        AudioUnitSetProperty (audioUnit, kAudioUnitProperty_AudioChannelLayout, kAudioUnitScope_Input,  0, &layout, sizeof (layout));
        AudioUnitSetProperty (audioUnit, kAudioUnitProperty_AudioChannelLayout, kAudioUnitScope_Output, 0, &layout, sizeof (layout));
    }

    {
        AURenderCallbackStruct inputProc;
        inputProc.inputProc = processStatic;
        inputProc.inputProcRefCon = this;
        AudioUnitSetProperty (audioUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &inputProc, sizeof (inputProc));
    }

    AudioUnitSetProperty (audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input,  0, &format, sizeof (format));
    AudioUnitSetProperty (audioUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &format, sizeof (format));

    AudioUnitInitialize (audioUnit);

    [(id)wrapper activateAudiobus: audioUnit];

    return true;
}

//void iOSAudioIODevice::fixAudioRouteIfSetToReceiver()
//{
//    CFStringRef audioRoute = 0;
//    if (getSessionProperty (kAudioSessionProperty_AudioRoute, audioRoute) == noErr)
//    {
//        NSString* route = (NSString*) audioRoute;
//
//        //DBG ("audio route: " + nsStringToJuce (route));
//
//        if ([route hasPrefix: @"Receiver"])
//            setSessionUInt32Property (kAudioSessionProperty_OverrideAudioRoute, kAudioSessionOverrideAudioRoute_Speaker);
//
//        CFRelease (audioRoute);
//    }
//}

//==============================================================================

class iOSAudioIODeviceType  : public AudioIODeviceType {
public:
    iOSAudioIODeviceType()  : AudioIODeviceType ("iOS Audio") {}
    ~iOSAudioIODeviceType() {}
    void scanForDevices() {}
    StringArray getDeviceNames (bool /*wantInputNames*/) const       { return StringArray ("iOS Audio"); }
    int getDefaultDeviceIndex (bool /*forInput*/) const              { return 0; }
    int getIndexOfDevice (AudioIODevice* d, bool /*asInput*/) const  { return d != nullptr ? 0 : -1; }
    bool hasSeparateInputsAndOutputs() const                         { return false; }

    AudioIODevice* createDevice (const String& outputDeviceName, const String& inputDeviceName) {
        if (outputDeviceName.isNotEmpty() || inputDeviceName.isNotEmpty())
        {
            //std::cout << "out: " << outputDeviceName << " --- in: " << inputDeviceName << std::endl;
            return new iOSAudioIODevice (outputDeviceName.isNotEmpty() ? outputDeviceName
                                                                       : inputDeviceName);
        }
        return nullptr;
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (iOSAudioIODeviceType)
};

//=============================================================================
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_iOSAudio() {
    return new iOSAudioIODeviceType();
}
