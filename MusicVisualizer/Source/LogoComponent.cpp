/*
  ==============================================================================

    LogoComponent.cpp
    Created: 24 Nov 2021 12:01:53pm
    Author:  Bevan Salter

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LogoComponent.h"

//==============================================================================
LogoComponent::LogoComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

LogoComponent::~LogoComponent()
{
}

void LogoComponent::paint (juce::Graphics& g)
{
    auto logo = juce::ImageCache::getFromMemory(BinaryData::LOGOTRANS_png, BinaryData::LOGOTRANS_pngSize);
    
    if (beatDetected == true)
    {
        auto logoScaled = logo.rescaled(400 * 1.5, 400 * 1.5);
        g.drawImageAt(logoScaled, 0, 0);
    }
    else
    {
        auto logoScaled = logo.rescaled(400 , 400);
        g.drawImageAt(logoScaled, 0, 0);
    }
    
}

void LogoComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
