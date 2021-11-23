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
    auto logo = juce::ImageCache::getFromMemory(BinaryData::LOGOTRANS_png, BinaryData::LOGOTRANS_pngSize);
    auto logoScaled = logo.rescaled(400, 400);
    
    g.drawImageAt(logoScaled, (getWidth() / 2) - 200, (getHeight() / 2) - 200);
    
    juce::Path centreCirclePath;
    centreCirclePath.addEllipse((getWidth() / 2) - 163, (getHeight() / 2) - 163, 326, 326);
    
    g.setColour(juce::Colours::orange);
    g.strokePath(centreCirclePath, juce::PathStrokeType (2.5f));
    
    juce::Point<float> pathPoint[static_cast<int>(centreCirclePath.getLength())];
    
    for (int i = 0; i < static_cast<float>(centreCirclePath.getLength()); i++)
        pathPoint[i] = centreCirclePath.getPointAlongPath(i);
    
    float lineAngleLeft = 0.f;
    float lineAngleRight = 2.f * M_PI;
    
    for (int i = 0; i < static_cast<int>(centreCirclePath.getLength() / 2); i++)
    {
        auto lineLeft = juce::Line<float>::fromStartAndAngle(pathPoint[i], /* scopeData[i] **/ 400.f, lineAngleLeft);
        if (i % 2 == 0)
            g.setColour(juce::Colour (0, 128, i / 2));
        
        g.drawLine(lineLeft);
        
        lineAngleLeft += (2.f * M_PI) / static_cast<float>(centreCirclePath.getLength());
    }
    
    int scopeDataPosition = 0;
    int colourPosition = 0;
    
    for (int i = static_cast<int>(centreCirclePath.getLength()); i > static_cast<int>(centreCirclePath.getLength()) / 2; i--)
    {
        auto lineRight = juce::Line<float>::fromStartAndAngle(pathPoint[i],/* scopeData[scopeDataPosition] **/ 400.f, lineAngleRight);
        if (colourPosition % 2 == 0)
            g.setColour(juce::Colour (0, 128, colourPosition / 2));
        
        g.drawLine(lineRight);
        
        lineAngleRight -= (2.f * M_PI) / static_cast<float>(centreCirclePath.getLength());
        
        scopeDataPosition++;
        colourPosition++;
    }
}

void FFTCircleComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    
}
