/*
  ==============================================================================

    MenuComponent.h
    Created: 26 Nov 2021 2:46:46pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/

struct PopupMenuLookAndFeel : public LookAndFeel_V4
{
    void drawPopupMenuColumnSeparatorWithOptions (Graphics& g,
                                                  const Rectangle<int>& bounds,
                                                  const PopupMenu::Options& opt)
    {
        if (auto* target = opt.getTargetComponent())
        {
            const auto baseColour = target->findColour (TextButton::buttonColourId);
            g.setColour (baseColour.brighter (0.4f));

            const float dashes[] { 5.0f, 5.0f };
            const auto centre = bounds.toFloat().getCentre();

            g.drawDashedLine ({ centre.withY ((float) bounds.getY()),
                                centre.withY ((float) bounds.getBottom()) },
                              dashes,
                              numElementsInArray (dashes),
                              3.0f);
        }
    }

    void drawPopupMenuBackgroundWithOptions (Graphics& g, int, int, const PopupMenu::Options& opt)
    {
        if (auto* target = opt.getTargetComponent())
        {
            g.fillAll (target->findColour (TextButton::buttonColourId));
        }
    }

    // Return the amount of space that should be left between popup menu columns.
    int getPopupMenuColumnSeparatorWidthWithOptions (const PopupMenu::Options&)
    {
        return 10;
    }
};

class MenuComponent  : public juce::Component
{
public:
    MenuComponent();
    ~MenuComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    Random randomColourGenerator;
    PopupMenuLookAndFeel popupLookAndFeel;

    TextButton shortMenuButton       { "Short" },
               longMenuButton        { "Long" },
               nestedMenusButton     { "Nested Sub-Menus" },
               multiColumnMenuButton { "Multi Column" },
               customItemButton      { "Custom Items" },
               fancyThemeButton      { "Fancy Theme with Column Breaks" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};
