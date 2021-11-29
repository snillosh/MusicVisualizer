/*
  ==============================================================================

    FreqMagnitudeDetector.h
    Created: 24 Nov 2021 12:36:37pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <array>



class FreqMagnitudeDetector
{
public:
    FreqMagnitudeDetector();
    ~FreqMagnitudeDetector();
    
    bool detectFreqsOverCertainLevel (std::array<float, 512> scopeDataArray);
    
    void setBeatDetectorAverage (std::array<float, 512> scopeDataArray);

    float getBeatDetecotrAverageLeft();
    
    float getBeatDetectorAverageRight();
    
    bool detectBeat(std::array<float, 512> scopeDataArray);
    
    int getScopeSizeForLevelDetector () {return scopeSize;}
    
    float getLevelDetectorThreshold () {return threshold;}
    void setLevelDetectorThreshold (float newThreshold) { threshold = newThreshold; }
    
private:
    //detectFreqsOverCertainLevel variables
    
    std::array <float, 20> levelDetectorArray;
    
    enum
    {
        scopeSize = 20
    };
    
    int levelDetectorArrayIndex = 0;
    float maxLevelDetected = 0;
    float threshold = 0.65f;
    
    //detectBeat variables
    
    //tells the class not to use magnitudes below this level
    float thresholdLeft = 0.f;
    float threshholdRight = 0.f;
    
    //the average level detected over the last 1.5s (90 frames)
    std::array<float, 90> averageArrayLeft;
    int averageArrayCounterLeft = 0;
    
    std::array<float, 90> averageArrayRight;
    int averageArrayCounterRight = 0;
};
