/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "FolderComponent.h"
#include "AppMenu.h"

/**
 * @file  MenuButton.h
 *
 * @brief 
 */
class AppMenu::MenuButton : public FolderComponent::ItemButton
{
public:
    enum ColourIds
    {
        textColourId = 0x1900200,
        backgroundColourId = 0x1900201,
        selectionColourId = 0x1900202,
        borderColourId = 0x1900203
    };

    MenuButton(MenuItem folderItem);

    virtual ~MenuButton() { }

    virtual MenuItem getMenuItem() const override;

    void setMenuItem(MenuItem newItem) override;

protected:
    virtual const juce::Rectangle<float>& getIconBounds() const = 0;

    virtual const juce::Rectangle<float>& getTitleBounds() const = 0;

    virtual bool shouldDrawBorder() const = 0;

    virtual bool shouldFillBackground() const = 0;
    
    virtual juce::Justification getTextJustification() const = 0;

private:
    void loadIcon();
   
    void updateFont();

    virtual void resized() override;
    
    virtual void paintButton
    (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown) override;

    int textWidth = 0;

    juce::Font titleFont;

    juce::Image icon;

    MenuItem menuItem;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
