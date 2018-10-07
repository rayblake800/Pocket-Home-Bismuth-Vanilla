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
    const AppMenuItem& getMenuItem() const;


    /**
     * @brief  Gets a PopupEditorComponent configured to edit this button's 
     *         data.
     *
     * @param onConfirm  A callback function that will run if changes are made 
     *                   and confirmed in the new editor.  This should be used
     *                   for updating button properties that are defined by its
     *                   AppMenuItem.
     *
     * @return           A new PopupEditorComponent, ready to be added to the 
     *                   screen.
     */
    AppMenuPopupEditor* getEditor
    (const std::function<void(AppMenuPopupEditor*) >& onConfirm);
    
    /**
     * @brief  Displays a confirmation window to the user requesting permission
     *         to delete this button, and runs a deletion callback function if 
     *         they confirm.
     *
     * @param onRemove  The callback function to run if the user clicks "OK".
     *                  This should remove the button from its parent component,
     *                  and delete the button's menu item data from its source.
     */
    void confirmRemoveButtonSource(const std::function<void() >& onRemove);

    /**
     * @brief Checks if this button is currently selected.
     * 
     * @return  True if and only if this button is selected.
     */
    bool isSelected() const;

    /**
     * @brief  Selects or un-selects this button.
     *
     * @param select   Sets the button as selected if true and unselected if
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
    const juce::Rectangle<float>& getTitleBounds() const;

    /**
     * @brief  Gets the button's icon bounds.
     *
     * @return   The area relative to this button's position where
     *           it will draw its icon.
     */
    const juce::Rectangle<float>& getIconBounds() const;

    /**
     * @brief  Gets the button's title font.
     *
     * @return  The font used to draw this button's title.
     */
    const juce::Font& getTitleFont() const;

    /**
     * @brief  Sets the bounds of the button's title.
     *
     * @param textBounds  The area relative to this button's position where
     *                    it will draw its title.
     */
    void setTitleBounds(const juce::Rectangle<float>& bounds);

    /**
     * @brief  Sets the bounds of the button's icon.
     *
     * @param bounds  The area relative to this button's position where
     *                it will draw its icon.
     */
    void setIconBounds(const juce::Rectangle<float>& bounds);

    /**
     * Sets if this button will draw an outline around its border.
     *
     * @param shouldDraw   True if and only if the outline should be drawn.
     */
    void setDrawBorder(const bool shouldDraw);

    /**
     * @brief  Sets if this button will fill in its background with its 
     *         background color.
     * 
     * @param shouldFill  True to fill in the entire background, false to just
     *                    fill in the area behind the button text.
     */
    void setFillBackground(const bool shouldFill);


    /**
     * @brief  Sets the button's title font.
     *
     * @param font   The font used to draw this button's title.
     */
    void setTitleFont(const juce::Font& font);

    /**
     * Sets the text justification of the button title.
     *
     * @param justification  This will be used to position button title within
     *                       the text bounds.
     */
    void setTextJustification(const juce::Justification justification);

private:
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
    void paintButton
    (juce::Graphics &g, bool isMouseOverButton, bool isButtonDown);

    /* Icon image to draw: */
    juce::Image appIcon;

    /* Bounds used to position and scale the button title and icon: */
    juce::Rectangle<float> titleBounds;
    juce::Rectangle<float> iconBounds;
    
    /* Title font and justification: */
    juce::Font titleFont;
    juce::Justification textJustification = juce::Justification::centredLeft;
    
    /* Pre-calculated text width */
    int textWidth;

    /* Background drawing options: */
    bool fillBackground = true;
    bool drawBorder = true;
    
    /* Menu item data object */
    AppMenuItem menuItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuButton)
};
