/*
  ==============================================================================

    ParticleComponent.cpp
    Created: 23 Nov 2021 2:29:38pm
    Author:  Bevan Salter

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParticleComponent.h"

//==============================================================================
ParticleComponent::ParticleComponent(Point<float> pos, float xDir, float yDir) : position(pos),
                                                                                 speed(Random::getSystemRandom().nextFloat() *  xDir,
                                                                                 Random::getSystemRandom().nextFloat() * yDir),
                                                                                 colour(Colours::white)
{
    setSize(7, 7);
    step();
}

ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::white);
    g.fillEllipse (2.0f, 2.0f, (float) getWidth() - 4.0f, (float) getHeight() - 4.0f);

    g.setColour (Colours::darkgrey);
    g.drawEllipse (2.0f, 2.0f, (float) getWidth() - 4.0f, (float) getHeight() - 4.0f, 1.0f);
}

bool ParticleComponent::step()
{
    position += speed;
    
    setCentrePosition((int) position.x, (int) position.y);
    
    if (auto* parent = getParentComponent())
        return isPositiveAndBelow(position.x, (float) parent->getWidth()) && position.y < (float) parent->getHeight();
    
    return position.y < 400.f && position.x >= -10.f;
}
