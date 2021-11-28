#pragma once

#include <JuceHeader.h>

#include<iostream>
#include<math.h>
#include<stdlib.h>

#include"ParticleComponent.h"
#include "FFTCircleComponent.h"
#include"FFTCircleData.h"
#include"LogoComponent.h"
#include"FreqMagnitudeDetector.h"
#include"MenuComponent.h"


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

private:
    //==============================================================================
    // Your private member variables go here...
    
    Image backgroundImage;
    Image backgroundImageScaled;
    
    //MenuComponent menuComponent;
    
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
    
    void updateParticlePositions(OwnedArray<ParticleComponent>& particleArray);
    void randomlyGenerateNewParticles(OwnedArray<ParticleComponent>& particleArray, int amountOfParticles, float xDir, float yDir, Point<float> newCentrePoint);
    
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    
    Point<float> centrePoint;
    OwnedArray<ParticleComponent> particlesTopLeft;
    OwnedArray<ParticleComponent> particlesTopRight;
    OwnedArray<ParticleComponent> particlesBottomLeft;
    OwnedArray<ParticleComponent> particlesBottomRight;
    
    FFTCircleComponent fftCircleComponent;
    LogoComponent logoComponent;
    
    FreqMagnitudeDetector beatDetector;
    
    FFTCircleData* circleDataPtr = &fftCircleComponent.fftCircleData;
    bool levelDectectorValue = false;
    bool beatDetectectorValue = false;
    
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
