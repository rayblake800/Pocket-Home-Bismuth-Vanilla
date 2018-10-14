// Disabled until redesign
#if 0
#pragma once
#include "ConfigurableComponent.h"
#include "AppMenuPopupEditor.h"
#include "AppMenuItem.h"

/** 
 * @file  AppMenuButton.h
 * 
 * @brief  A custom button type used for launching applications and opening 
 *         application directories. 
 * 
 * Each button has an associated AppMenuItem that can be used for accessing and 
 * modifying the button's menu data.  This is used to load a button icon, set
 * a button title, and define how the button acts when clicked.
 * 
 * @see AppMenuComponent, AppMenuItem
 */

class AppMenuButton : public juce::Button, public juce::ReferenceCountedObject
{
public:
    typedef juce::ReferenceCountedObjectPtr<AppMenuButton> Ptr;

    enum ColourIds
    {
        textColourId = 0x1900200,
        backgroundColourId = 0x1900201,
        selectionColourId = 0x1900202,
        borderColourId = 0x1900203
    };

    /**
     * @brief  Creates a new AppMenuButton component for a menu item.
     *
     * @param menuItem    The new button's menu data source.
     */
    AppMenuButton(AppMenuItem menuItem);

    virtual ~AppMenuButton() { }

    /**
     * @brief   Gets this button's menu data.
     * 
     * @return  A copy of this button's internal AppMenuItem.
     */
    AppMenuItem getMenuItem() const;

    /**
     * @brief  Replaces the menu item used by this button.
     *
     * @param newItem  The new menu item to apply.
     */
    void setMenuItem(const AppMenuItem newItem);

    /**
     * @brief  Checks if this button is currently selected.
     * 
     * @return  Whether this menu button is selected.
     */
    bool isSelected() const;

    /**
     * @brief  Selects or un-selects this button.
     *
     * @param select   Sets the button as selected if true or unselected if
     *                 false.
     */
    void setSelected(bool select);

protected:
    /**
     * @brief  Triggers whenever the button is selected or unselected.
     */
    virtual void selectionStateChanged() { }

    /**
     * @brief  Requests an icon from the icon thread.
     *
     * @param icon   An icon's full path, or the name of an icon file located
     *               in common icon directories.
     */
    void loadIcon(const juce::String& icon);

    /**
     * @brief  Reloads this button's data from its menu item.
     */
    virtual void reloadDataFromSource();

    /**
     * @brief  Gets the button's title bounds.
     *
     * @return   The area relative to this button's position where
     *           it will draw its title.
     */
    virtual const juce::Rectangle<float>& getTitleBounds() const = 0;

    /**
     * @brief  Gets the button's icon bounds.
     *
     * @return   The area relative to this button's position where
     *           it will draw its icon.
     */
    virtual const juce::Rectangle<float>& getIconBounds() const = 0;

    /**
     * @brief  Gets the button's title font.
     *
     * @return  The font used to draw this button's title.
     */
    virtual const juce::Font& getTitleFont() const = 0;

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

protected:
    /**
     * @brief  Performs any additional updates needed whenever the button's
     *         component bounds change.
     *
     * AppMenuButton subclasses should override this instead of resized().
     */
    virtual void menuButtonResized() { }

private:
    /**
     * @brief  Calculates text size and calls menuButtonResized when the 
     *         button's component bounds change.
     */
    virtual void resized() final override;
    
    /**
     * @brief  Custom button painting method called by juce library code.
     * 
     * @param g                   Used for performing graphics operation.
     *
     * @param isMouseOverButton   Indicates if the mouse is over the button
     *                            component.
     * 
     * @param isButtonDown        Indicates if the user is currently holding
     *                            down the button.
     */
    virtual void paintButton
    (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown) override;

    /* Icon image to draw: */
    juce::Image appIcon;
    
    /* Pre-calculated text width */
    int textWidth = 0;
    
    /* Menu item data object */
    AppMenuItem menuItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuButton)
};

//Disabled until redesign
#endif
