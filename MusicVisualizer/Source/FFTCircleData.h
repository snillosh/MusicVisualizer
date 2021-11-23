/*
  ==============================================================================

    FFTCircleData.h
    Created: 23 Nov 2021 2:28:58pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FFTCircleData
{
public:
    FFTCircleData();
    ~FFTCircleData();
    
    void pushNextSampleIntoFifo (float sample) noexcept;
    void drawNextFrameOfSpectrum();
    void processAudioBlock(float inputSample);
    float getScopeDataAtIndex(int index);
    
    enum
    {
        fftOrder  = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };
    
    float scopeData [scopeSize];
    bool nextFFTBlockReady = false;
    
private:
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    
    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
    
};
