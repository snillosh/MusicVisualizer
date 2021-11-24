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

bool FreqMagnitudeDetector::detectBeat(float inputScopeData)
{
    if (inputScopeData >= threshold)
    {
        //DBG("Beat");
        return true;
    }
        
    else
    {
        //DBG("No Beat");
        return false;
    }
}
