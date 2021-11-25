/*
  ==============================================================================

    FFTCircleData.cpp
    Created: 23 Nov 2021 2:28:58pm
    Author:  Bevan Salter

  ==============================================================================
*/

#include "FFTCircleData.h"


FFTCircleData::FFTCircleData() : forwardFFT(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::blackman)
{
    
}

FFTCircleData::~FFTCircleData()
{
    
}

void FFTCircleData::pushNextSampleIntoFifo (float sample) noexcept
{
    if (fifoIndex == fftSize)
    {
        if (! nextFFTBlockReady)
        {
            juce::zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }
 
        fifoIndex = 0;
    }
 
    fifo[fifoIndex++] = sample;
}
void FFTCircleData::drawNextFrameOfSpectrum()
{
    // first apply a windowing function to our data
        window.multiplyWithWindowingTable (fftData, fftSize);       // [1]
     
        // then render our FFT data..
        forwardFFT.performFrequencyOnlyForwardTransform (fftData);  // [2]
     
        auto mindB = -100.0f;
        auto maxdB =    0.0f;
     
        for (int i = 0; i < scopeSize; ++i)                         // [3]
        {
            auto skewedProportionX = 1.0f - std::exp (std::log (1.0f - (float) i / (float) scopeSize) * 0.2f);
            auto fftDataIndex = juce::jlimit (0, fftSize / 2, (int) (skewedProportionX * (float) fftSize * 0.5f));
            auto level = juce::jmap (juce::jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (fftData[fftDataIndex])
                        - juce::Decibels::gainToDecibels ((float) fftSize)),
                        mindB, maxdB, 0.0f, 1.0f);
     
            scopeData[i] = level;                                   // [4]
        }
}

void FFTCircleData::processFFTSampleBySample(float inputSample)
{
    pushNextSampleIntoFifo (inputSample);
}

float FFTCircleData::getScopeDataAtIndex(int index)
{
    return scopeData[index];
}
