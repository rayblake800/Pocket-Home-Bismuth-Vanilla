/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "FolderComponent.h"
#include "AppMenu.h"

/**
 * @file  MenuButton.h
 *
 * @brief  Represents a single menu item as a juce button component.
 */
class AppMenu::MenuButton : public FolderComponent::ItemButton,
private MenuItem::Listener
{
public:
    enum ColourIds
    {
        textColourId = 0x1900200,
        backgroundColourId = 0x1900201,
        selectionColourId = 0x1900202,
        borderColourId = 0x1900203
    };

    /**
     * @brief  Creates a new MenuButton component for a menu item.
     *
     * @param menuItem   The new button's menu data source.
     */
    MenuButton(MenuItem menuItem);

    virtual ~MenuButton() { }

protected:
    /**
     * @brief  Sets the location relative to the button's bounds where the
     *         button's icon will be drawn.
     *
     * @param newBounds  The new icon bounds to save.
     */
    virtual void setIconBounds(const juce::Rectangle<float>& newBounds);

    /**
     * @brief  Sets the location relative to the button's bounds where the
     *         button's title will be printed.
     *
     * @param newBounds  The new title bounds to save.
     */
    virtual void setTitleBounds(const juce::Rectangle<float>& newBounds);

    /**
     * @brief  Re-calculates and saves the button's icon and title bounds.
     */
    virtual void updateButtonLayout() = 0;
    
    /**
     * @brief  Checks if this button will draw an outline around its border.
     *
     * @return  Whether the border outline will be drawn.
     */
    virtual bool shouldDrawBorder() const = 0;

    /**
     * @brief  Checks if this button will fill in its background with its 
     *         background color.
     * 
     * @return  True to fill in the entire background, false to just fill in the
     *          area behind the button text.
     */
    virtual bool shouldFillBackground() const = 0;
        
    /**
     * @brief  Gets the text justification of the button title.
     *
     * @return   The justification type used to position the button title within
     *           the text bounds.
     */
    virtual juce::Justification getTextJustification() const = 0;

private:
    /**
     * @brief  Updates the component if necessary whenever its menu data
     *         changes.
     *
     * @param changedField  The updated data field.
     */
    virtual void dataChanged(MenuItem::DataField changedField);

    /**
     * @brief  Reloads the button icon image if necessary.
     */
    void loadIcon();
   
    /**
     * @brief  Updates the button's title font to fit the current title bounds.
     */
    void updateFont();

    /**
     * @brief  Runs whenever the menu button is resized, just before
     *         recalculating icon bounds, text bounds, and font size.  
     *         
     * MenuButton subclasses should override this method instead of resized().
     */
    virtual void menuButtonResized() { }

    /**
     * @brief  Calls menuButtonResized() and re-calculates title and icon layout
     *         whenever the button's bounds change.
     */
    virtual void resized() final override;
    
    /**
     * @brief  Draws the button object.
     *
     * @param g                  The juce graphics object used to access
     *                           draw commands.
     *
     * @param isMouseOverButton  Whether the mouse is currently over the button.
     *
     * @param isButtonDown       Whether the button is currently being held
     *                           down.
     *
     */
    virtual void paintButton
    (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown) override;

    /* Pre-calculated title text width */
    int textWidth = 0;

    /* Area relative to the button's bounds where the icon will be drawn. */
    juce::Rectangle<float> iconBounds;

    /* Area relative to the button's bounds where the title will be printed. */
    juce::Rectangle<float> titleBounds;

    /* The font used to print the button's title */
    juce::Font titleFont;

    /* The icon to draw on this button */
    juce::Image icon;

    /* The name or path of the last loaded icon */
    juce::String iconName;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
