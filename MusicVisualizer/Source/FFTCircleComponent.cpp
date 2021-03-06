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
    for (int i = 0; i < FFTCircleData::scopeSize; i++)
    {
        scopeData[i] = 0;
    }
}

FFTCircleComponent::~FFTCircleComponent()
{
}

void FFTCircleComponent::paint (juce::Graphics& g)
{
    for (int i = 0; i < FFTCircleData::scopeSize; i++)
    {
        scopeData[i] = fftCircleData.getScopeDataAtIndex(i);
        if (scopeData[i] < 0)
        {
            scopeData[i] = 0;
        }
    }
    
    juce::Path centreCirclePath;
    juce::Path FFTConnectingLineLeft;
    juce::Path FFTConnectingLineRight;
    
    centreCirclePath.addEllipse((getWidth() / 2) - 81.5f, (getHeight() / 2) - 81.5f, 163, 163);
    
    g.setColour(juce::Colours::orange);
    g.strokePath(centreCirclePath, juce::PathStrokeType (2.5f));
    
    juce::Point<float> pathPoint[static_cast<int>(centreCirclePath.getLength())];
    
    for (int i = 0; i < static_cast<float>(centreCirclePath.getLength()); i++)
        pathPoint[i] = centreCirclePath.getPointAlongPath(i);
    
    float lineAngleLeft = 0.f;
    float lineAngleRight = 2.f * M_PI;
    
    Line<float> previousLineLeft;
    Line<float> previousLineRight;
    
    for (int i = 0; i < static_cast<int>(centreCirclePath.getLength() / 2); i++)
    {
        auto lineLeft = juce::Line<float>::fromStartAndAngle(pathPoint[i], scopeData[i] * 400.f, lineAngleLeft);
        if (i % 2 == 0)
            g.setColour(juce::Colour (0, 128, i / 2));
        
        //g.drawLine(lineLeft);
        
        if (i == 0)
        {
            FFTConnectingLineLeft.startNewSubPath(lineLeft.getEnd());
        }
        else
        {
            FFTConnectingLineLeft.lineTo(previousLineLeft.getEnd());
        }
        
        previousLineLeft = lineLeft;
        
        
        lineAngleLeft += (2.f * M_PI) / static_cast<float>(centreCirclePath.getLength());
    }
    for (int i = static_cast<int>(centreCirclePath.getLength()) / 2; i > 0; i--)
    {
        FFTConnectingLineLeft.lineTo(pathPoint[i]);
    }
    FFTConnectingLineLeft.closeSubPath();
    
    
    int scopeDataPosition = 0;
    int colourPosition = 0;
    
    for (int i = static_cast<int>(centreCirclePath.getLength()); i > static_cast<int>(centreCirclePath.getLength()) / 2; i--)
    {
        auto lineRight = juce::Line<float>::fromStartAndAngle(pathPoint[i], scopeData[scopeDataPosition] * 400.f, lineAngleRight);
        if (colourPosition % 2 == 0)
            g.setColour(juce::Colour (0, 128, colourPosition / 2));
        
        //g.drawLine(lineRight);
        
            if (i == static_cast<int>(centreCirclePath.getLength()))
                FFTConnectingLineRight.startNewSubPath(lineRight.getEnd());
            else
            {
                FFTConnectingLineRight.lineTo(previousLineRight.getEnd());
            }
        
        previousLineRight = lineRight;
        
        lineAngleRight -= (2.f * M_PI) / static_cast<float>(centreCirclePath.getLength());
        
        scopeDataPosition++;
        colourPosition++;
    }
    for (int i = static_cast<int>(centreCirclePath.getLength()) / 2; i < static_cast<int>(centreCirclePath.getLength()); i++)
    {
        FFTConnectingLineRight.lineTo(pathPoint[i]);
    }
    FFTConnectingLineRight.closeSubPath();
    
    if (gradiantCounter == 15) {
        colgrad = (ColourGradient (getRandomColour(), getWidth() / 2, getHeight() / 2,
                                           getRandomColour(), 0, 1300, true));
        gradiantCounter = 0;
    }
    else
        gradiantCounter++;
    
    g.setGradientFill(colgrad);
    
    g.fillPath(FFTConnectingLineLeft);
    g.fillPath(FFTConnectingLineRight);
    
    //drawFrame(g);
}

void FFTCircleComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    
}
void FFTCircleComponent::drawFrame (juce::Graphics& g)
{
    for (int i = 1; i < FFTCircleData::scopeSize; ++i)
    {
        auto width  = getLocalBounds().getWidth();
        auto height = getLocalBounds().getHeight();
        g.setColour(Colours::white);
        g.drawLine ({ (float) juce::jmap (i - 1, 0, FFTCircleData::scopeSize - 1, 0, width),
                                juce::jmap (scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f),
                        (float) juce::jmap (i,     0, FFTCircleData::scopeSize - 1, 0, width),
                                juce::jmap (scopeData[i],     0.0f, 1.0f, (float) height, 0.0f) });
    }
    for (int i = 7; i < 20; ++i)
    {
        auto width  = getLocalBounds().getWidth();
        auto height = getLocalBounds().getHeight();
        g.setColour(Colours::green);
        g.drawLine ({ (float) juce::jmap (i - 1, 0, FFTCircleData::scopeSize - 1, 0, width),
                                juce::jmap (scopeData[i - 1], 0.0f, 1.0f, (float) height, 0.0f),
                        (float) juce::jmap (i,     0, FFTCircleData::scopeSize - 1, 0, width),
                                juce::jmap (scopeData[i],     0.0f, 1.0f, (float) height, 0.0f) });
    }
}

Colour FFTCircleComponent::getRandomColour()
{
    auto& random = juce::Random::getSystemRandom();
     
    return juce::Colour ((juce::uint8) random.nextInt (256),
                         (juce::uint8) random.nextInt (256),
                         (juce::uint8) random.nextInt (256));
}
