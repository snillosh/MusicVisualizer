/*
  ==============================================================================

    FreqMagnitudeDetector.cpp
    Created: 24 Nov 2021 12:36:37pm
    Author:  Bevan Salter

  ==============================================================================
*/

#include "FreqMagnitudeDetector.h"

FreqMagnitudeDetector::FreqMagnitudeDetector()
{
    
}

FreqMagnitudeDetector::~FreqMagnitudeDetector()
{
    
}

bool FreqMagnitudeDetector::detectFreqsOverCertainLevel(float inputScopeData)
{
    if (inputScopeData >= threshold)
        return true;
    else
        return false;
    
    
//    if (levelDetectorArrayIndex == scopeSize)
//    {
//        levelDetectorArrayIndex = 0;
//
//        for (int i = 0; i < scopeSize; i++)
//        {
//            if (levelDetectorArray[i] >= maxLevelDetected)
//            {
//                maxLevelDetected = levelDetectorArray[i];
//            }
//        }
//        if (maxLevelDetected >= threshold)
//        {
//            maxLevelDetected = 0;
//            return true;
//        }
//        else
//        {
//            maxLevelDetected = 0;
//            return false;
//        }
//    }
//    else
//    {
//        levelDetectorArray[levelDetectorArrayIndex] = inputScopeData;
//        levelDetectorArrayIndex++;
//        return false;
//    }
}

bool FreqMagnitudeDetector::detectBeat (float freq1, float freq2)
{
    return false;
}
