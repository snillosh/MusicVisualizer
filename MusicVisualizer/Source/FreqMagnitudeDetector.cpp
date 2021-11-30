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
    for ( int i = 0; i < averageArrayLeft.size(); i++)
    {
        averageArrayLeft[i] = 0;
        averageArrayRight[i] = 0;
    }
    
}

FreqMagnitudeDetector::~FreqMagnitudeDetector()
{
    
}

bool FreqMagnitudeDetector::detectFreqsOverCertainLevel(std::array<float, 256> scopeDataArray)
{
    for (int i = 0; i < 20; i++)
    {
        levelDetectorArray[i] = scopeDataArray[i];
        
        if (levelDetectorArray[i] >= maxLevelDetected)
        {
            maxLevelDetected = levelDetectorArray[i];
        }
    }
    if (maxLevelDetected >= threshold)
    {
        maxLevelDetected = 0;
        return true;
    }
    else
    {
        maxLevelDetected = 0;
        return false;
    }
}

void FreqMagnitudeDetector::setBeatDetectorAverage (std::array<float, 256> scopeDataArray)
{
    if (averageArrayCounterLeft == averageArrayLeft.size())
    {
        averageArrayLeft[averageArrayCounterLeft] = scopeDataArray[7];
        averageArrayCounterLeft = 0;
    }
    else
    {
        averageArrayLeft[averageArrayCounterLeft] = scopeDataArray[7];
        averageArrayCounterLeft++;
    }
    
    if (averageArrayCounterRight == averageArrayRight.size())
    {
        averageArrayRight[averageArrayCounterRight] = scopeDataArray[20];
        averageArrayCounterRight = 0;
    }
    else
    {
        averageArrayRight[averageArrayCounterRight] = scopeDataArray[20];
        averageArrayCounterRight++;
    }
}

float FreqMagnitudeDetector::getBeatDetecotrAverageLeft()
{
    float beatDetectorAverageLeftSum = 0;
    
    for (int i = 0; i < averageArrayLeft.size(); i++)
    {
        beatDetectorAverageLeftSum += averageArrayLeft[i];
    }
    //DBG("Beat Average Left: " << beatDetectorAverageLeftSum / static_cast<float> (averageArrayLeft.size()));
    return beatDetectorAverageLeftSum / static_cast<float> (averageArrayLeft.size());
}

float FreqMagnitudeDetector::getBeatDetectorAverageRight()
{
    float beatDetectorAverageRightSum = 0;
    
    for (int i = 0; i < averageArrayRight.size(); i++)
    {
        beatDetectorAverageRightSum += averageArrayRight[i];
    }
    return beatDetectorAverageRightSum / static_cast<float> (averageArrayRight.size());
    
}

bool FreqMagnitudeDetector::detectBeat(std::array<float, 256> scopeDataArray)
{
    setBeatDetectorAverage(scopeDataArray);
    
    float averageValueLeft = getBeatDetecotrAverageLeft();
    float averageValueRight = getBeatDetectorAverageRight();
    
    float beatMinValueLeft = averageValueLeft * 1.15f;
    float beatMinValueRight = averageValueRight * 1.15f;
    
    float currentInputValueAtFreqLeft = scopeDataArray[7];
    float currentInputValueAtFreqRight = scopeDataArray[20];
    
    if (currentInputValueAtFreqLeft >= beatMinValueLeft || currentInputValueAtFreqRight >= beatMinValueRight)
    {
        //DBG("Beat");
        return true;
    }
        
    else
    {
        //DBG("-------");
        return false;
    }
        
}
