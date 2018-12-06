#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu/Components/MenuButton.h
 *
 * @brief  Represents a single menu item as a juce::Button component.
 */

#include "IconLoader.h"
#include "AppMenu/Data/MenuItem.h"

namespace AppMenu { class MenuButton; }

/**
 *  The MenuButton is responsible for displaying a MenuItem's data to the user.
 * It draws the menu item's icon, prints its title, and draws itself differently
 * depending on whether it is the selected menu item in its folder. The 
 * MenuButton tracks changes to its MenuItem, automatically updating itself
 * when relevant menu data changes.
 *
 *  MenuButton is an abstract class. Each AppMenu Format should have its own
 * MenuButton implementation that defines how the button's title and icon are
 * placed, when the background or outline should be drawn, and optionally 
 * updates the size of the button's font.
 */
class AppMenu::MenuButton : public juce::Button, private MenuItem::Listener
{
public:
    /**
     * @brief  ID values used to load the component's colors from the 
     *         application's LookAndFeel object.
     */
    enum ColourIds
    {
        /* The color used to print the menu item's title */
        textColourId = 0x1900200,
        /* The color that may be filled in for the menu item's background */
        backgroundColourId = 0x1900201,
        /* The color used for the background when the menu item is selected */
        selectionColourId = 0x1900202,
        /* The color that may be used to draw the menu item's border */
        borderColourId = 0x1900203
    };

    /**
     * @brief  Creates a new MenuButton component for a menu item.
     *
     * @param menuItem  The new button's menu data source.
     */
    MenuButton(MenuItem menuItem);

    /**
     * @brief  Cancels any pending icon loading callback.
     */
    virtual ~MenuButton();

    /**
     * @brief  Checks if this button is the selected button in its folder.
     *
     * @return  Whether the button is selected. 
     */
    bool isSelected() const;

    /**
     * @brief  Selects or deselects this button.
     *
     * @param isSelected  Whether the button should be selected.
     */
    void setSelected(const bool isSelected);

    /**
     * @brief  Gets the MenuItem that defines this button.
     *
     * @return  The MenuItem used to create the MenuButton. 
     */
    MenuItem getMenuItem() const;

    /**
     * @brief  Gets the width of the button's title string.
     *
     * @return   The number of horizontal pixels needed to print the full button
     *           title with the current title font.  This may be larger or
     *           smaller than the width of the title bounds.
     */
    int getTitleWidth() const;

protected:
    /**
     * @brief  Recalculates and saves the menu button title bounds.
     *
     * The title bounds is the area relative to the component's bounds that the
     * menu item title will be drawn within.
     */
    void updateTitleBounds();

    /**
     * @brief  Recalculates and saves the menu button icon bounds.
     *
     * The icon bounds is the area relative to the component's bounds that the
     * menu item icon will be drawn within.
     */
    void updateIconBounds();

    /**
     * @brief  Recreates and saves the font used to draw the menu button's
     *         title.
     *
     * This resizes the font to fit within the current title bounds.
     */
    void updateFont(); 
    
private:
    /**
     * @brief  Finds the area relative to the menu button's bounds where the
     *         title should be drawn.
     *
     * @return  An appropriate title bounds area for the current button size
     *          and the needs of the MenuButton subclass implementing this
     *          method.
     */
    virtual juce::Rectangle<float> findTitleBounds() const = 0;

    /**
     * @brief  Finds the area relative to the menu button's bounds where the
     *         icon should be drawn.
     *
     * @return  An appropriate icon bounds area for the current button size
     *          and the needs of the MenuButton subclass implementing this
     *          method.
     */
    virtual juce::Rectangle<float> findIconBounds() const = 0;

    /**
     * @brief  Updates the title font to fit the current title bounds.
     *
     * @param titleBounds  The area the title will be drawn within.
     *
     * @return             An appropriate font for drawing the title within the
     *                     given bounds.
     */
    virtual juce::Font findTitleFont(const juce::Rectangle<float>& titleBounds)
        const;

    /**
     * @brief  Finds the width of the background area that will be drawn behind
     *         the button's title.
     *
     * @param titleBounds  The title's bounding box.
     *
     * @param titleFont    The font that will be used to draw the title.
     *
     * @return             The width to draw the title background, or any value
     *                     less than or equal to zero to skip drawing the title
     *                     background.
     */
    virtual int findTitleBGWidth(const juce::Rectangle<float>& titleBounds,
            const juce::Font& titleFont) const;
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

    /**
     * @brief  Updates the component if necessary whenever its menu data
     *         changes.
     *
     * @param changedField  The updated data field.
     */
    virtual void dataChanged(MenuItem::DataField changedField) final override;

    /**
     * @brief  Reloads the button icon image if necessary.
     */
    void loadIcon();

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
    (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
    final override;
    
    /* The MenuItem that defines this button's data. */
    MenuItem menuItem;

    /* Whether the button is the selected button in its folder. */
    bool selected = false;

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

    /* ID used to cancel pending icon requests if necessary */
    IconLoader::RequestID iconCallbackID = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuButton)
};
