/*
  ==============================================================================

    LogoComponent.h
    Created: 24 Nov 2021 12:01:53pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LogoComponent  : public juce::Component
{
public:
    LogoComponent();
    ~LogoComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogoComponent)
};
