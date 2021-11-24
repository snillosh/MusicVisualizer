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
    
    centreCirclePath.addEllipse((getWidth() / 2) - 163, (getHeight() / 2) - 163, 326, 326);
    
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
                FFTConnectingLineLeft.startNewSubPath(lineLeft.getEnd());
            else
            {
                FFTConnectingLineLeft.lineTo(previousLineLeft.getEnd());
            }
        previousLineLeft = lineLeft;
        
        
        lineAngleLeft += (2.f * M_PI) / static_cast<float>(centreCirclePath.getLength());
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
    FFTConnectingLineRight.closeSubPath();
    
    //auto fillType = FillType();
    //auto colGrad = ColourGradient(Colours::white, getWidth() / 2 , getHeight() / 2 ,Colours::red, getWidth() / 2, getHeight() / 2, true);
    //colGrad.addColour(0.5, Colours::red);
    //colGrad.addColour(0.2, Colours::white);
    //g.setGradientFill(colGrad);
    //fillType.setColour(Colours::red);
    //fillType.setGradient(colGrad);
    //g.setFillType(fillType);
    //g.setFillType(colGrad);
    
    
    g.setColour(juce::Colours::darkred);
    //g.strokePath(FFTConnectingLineLeft, juce::PathStrokeType (1.f));
    //g.strokePath(FFTConnectingLineRight, juce::PathStrokeType (1.f));
    g.fillPath(FFTConnectingLineLeft);
    g.fillPath(FFTConnectingLineRight);
}

void FFTCircleComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    
}
