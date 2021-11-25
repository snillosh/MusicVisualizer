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
    
    bool detectFreqsOverCertainLevel (float inputScopeData);
    bool detectBeat (float freq1, float freq2);
    
    int getScopeSizeForLevelDetector () {return scopeSize;}
    
private:
    //detectFreqsOverCertainLevel variables
    
    enum
    {
        scopeSize = 20
    };
    
    float levelDetectorArray[scopeSize];
    int levelDetectorArrayIndex = 0;
    float maxLevelDetected = 0;
    float threshold = 0.65f;
    
    //detectBeat variables
    
    //tells the class not to use magnitudes below this level
    float thresholdLeft = 0.f;
    float threshholdRight = 0.f;
    
    //the average level detected over the last 1.5s (90 frames)
    float averageLeft = 0.f;
    float averageRight = 0.f;
    
    //the level at which a positive beat is detected, returning a true from the function
    float beatMinimumLeft = averageLeft * 2.2f;
    float beatMinimumRight = averageRight * 2.2f;
};
