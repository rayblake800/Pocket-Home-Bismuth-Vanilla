/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/MenuComponents/MenuButton.h"

class AppMenu::Scrolling::MenuButton : public AppMenu::MenuButton
{
public:
    MenuButton(MenuItem menuItem);

    virtual ~MenuButton() { }

private:
    virtual juce::Rectangle<float> findTitleBounds() const final override;

    virtual juce::Rectangle<float> findIconBounds() const final override;

    virtual bool shouldDrawBorder() const final override;

    virtual bool shouldFillBackground() const final override;

    virtual juce::Justification getTextJustification() const final override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
