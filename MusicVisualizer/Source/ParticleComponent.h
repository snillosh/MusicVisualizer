/*
  ==============================================================================

    ParticleComponent.h
    Created: 23 Nov 2021 2:29:38pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ParticleComponent  : public juce::Component
{
public:
    ParticleComponent();
    ~ParticleComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParticleComponent)
};
