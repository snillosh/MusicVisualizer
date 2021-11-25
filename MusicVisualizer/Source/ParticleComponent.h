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
    ParticleComponent(Point<float> pos, float xDir, float yDir);
    ~ParticleComponent() override;

    void paint (juce::Graphics&) override;
    
    bool step();
    
    Point<float> position, speed;
    Colour colour;
    float xDir, yDir;
    bool beatDetected = false;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParticleComponent)
};
