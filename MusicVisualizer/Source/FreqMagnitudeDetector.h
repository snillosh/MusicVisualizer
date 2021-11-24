/*
  ==============================================================================

    FreqMagnitudeDetector.h
    Created: 24 Nov 2021 12:36:37pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



class FreqMagnitudeDetector
{
public:
    FreqMagnitudeDetector();
    ~FreqMagnitudeDetector();
    
    bool detectBeat (float inputScopeData);
    
    bool magThresholdReached = false;
    
    int scopeSize = 20;
    
private:
    float lowScopeData[20];
    float threshold = 0.6f;
    float thresholdLeft = 0.f;
    float threshholdRight = 0.f;
    
    float averageLeft = 0.f;
    float averageRight = 0.f;
    
    float beatMinimumLeft = 0.f;
    float beatMinimumRight = 0.f;
};
