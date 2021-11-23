/*
  ==============================================================================

    FFTCircleComponent.h
    Created: 23 Nov 2021 2:29:14pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "FFTCircleData.h"

//==============================================================================
/*
*/
class FFTCircleComponent  : public juce::Component
{
public:
    FFTCircleComponent();
    ~FFTCircleComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    FFTCircleData fftCircleData;

private:
    
    float scopeData[FFTCircleData::scopeSize];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTCircleComponent)
};
