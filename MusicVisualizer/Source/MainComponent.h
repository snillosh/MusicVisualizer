#pragma once

#include <JuceHeader.h>

#include<iostream>
#include<math.h>
#include<stdlib.h>

#include"ParticleComponent.h"
#include "FFTCircleComponent.h"
#include"FFTCircleData.h"
#include"LogoComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::ChangeListener,
                       public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void pushNextSampleIntoFifo (float sample) noexcept;
    void drawNextFrameOfSpectrum();
    
    enum
    {
        fftOrder  = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

private:
    //==============================================================================
    // Your private member variables go here...
    
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };
    
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void changeState (TransportState newState);
    
    void timerCallback() override;
    void drawFrame (juce::Graphics& g);
    
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    
    std::unique_ptr<juce::FileChooser> chooser;
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    
    Point<float> centrePoint;
    OwnedArray<ParticleComponent> particlesTopLeft;
    OwnedArray<ParticleComponent> particlesTopRight;
    OwnedArray<ParticleComponent> particlesBottomLeft;
    OwnedArray<ParticleComponent> particlesBottomRight;
    
    FFTCircleComponent fftCircleComponent;
    LogoComponent logoComponent;
    
    FFTCircleData* circleDataPtr = &fftCircleComponent.fftCircleData;
    
    AudioBuffer<float> fftAudioBuffer;
    
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
