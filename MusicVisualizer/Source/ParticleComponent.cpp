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
    setSize(20, 20);
    step();
}

ParticleComponent::~ParticleComponent()
{
}

void ParticleComponent::paint (juce::Graphics& g)
{
    g.setColour(Colours::darkgrey);
    g.drawEllipse(10.f + ( -5 * speed.getX()), 10.f + ( -5 * speed.getY()), 3.f * 0.2f, 3.f * 0.2f, 1.0f);
    
    g.drawEllipse(10.f + ( -4 * speed.getX()), 10.f + ( -4 * speed.getY()), 3.f * 0.4f, 3.f * 0.4f, 1.0f);
    
    g.drawEllipse(10.f + ( -3 * speed.getX()), 10.f + ( -3 * speed.getY()), 3.f * 0.6f, 3.f * 0.6f, 1.0f);
    
    g.drawEllipse(10.f + ( -2 * speed.getX()), 10.f + ( -2 * speed.getY()), 3.f * 0.8f, 3.f * 0.8f, 1.0f);
    
    g.drawEllipse(10.f + ( -1 * speed.getX()), 10.f + ( -1 * speed.getY()), 3.f, 3.f, 1.0f);
    
    g.setColour (juce::Colours::white);
    g.fillEllipse (10.0f, 10.0f, 3.f, 3.0f);

    g.setColour (Colours::grey);
    g.drawEllipse (10.0f, 10.0f, 3.0f, 3.f, 1.0f);
}

bool ParticleComponent::step()
{
    if (beatDetected == true)
    {
        position += speed * 3.0f;
    }
    else
    {
        position += speed;
    }
    
    setCentrePosition((int) position.x, (int) position.y);
    
    if (auto* parent = getParentComponent())
        return isPositiveAndBelow(position.x, (float) parent->getWidth()) && position.y < (float) parent->getHeight();
    
    return position.y < 400.f && position.x >= -10.f;
}
