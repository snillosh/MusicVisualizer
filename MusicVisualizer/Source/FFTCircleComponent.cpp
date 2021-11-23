/*
  ==============================================================================

    FFTCircleComponent.cpp
    Created: 23 Nov 2021 2:29:14pm
    Author:  Bevan Salter

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FFTCircleComponent.h"

//==============================================================================
FFTCircleComponent::FFTCircleComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

FFTCircleComponent::~FFTCircleComponent()
{
}

void FFTCircleComponent::paint (juce::Graphics& g)
{
    g.setColour(Colours::red);
    g.drawEllipse((getWidth() / 2) - 100, (getHeight() / 2) - 100, 200, 200, 5.f);
}

void FFTCircleComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
