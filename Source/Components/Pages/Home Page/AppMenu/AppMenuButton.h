#pragma once
#include "ConfigurableComponent.h"
#include "AppMenuPopupEditor.h"
#include "AppMenuItem.h"

/** 
 * @file AppMenuButton.h
 * 
 * @brief AppMenuButton is a custom button type for launching applications and 
 * opening application directories. 
 * 
 * Each button displays an icon and a title.
 * Each button has an associated AppMenuItem that can be used for accessing and 
 * modifying the button's menu data.  
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
     * Create a new AppMenuButton component for a menu item.
     *
     * @param menuItem    This is set as the button's menu data source.
     *
     * @param name        Sets the button's internal component name.
     */
    AppMenuButton(AppMenuItem::Ptr menuItem, 
            juce::String name = juce::String());

    virtual ~AppMenuButton() { }

    /**
     * Get this button's menu data.
     * 
     * @return  A pointer to this button's internal AppMenuItem.  This pointer
     *          will be valid as long as this AppMenuButton still exists.
     */
    AppMenuItem::Ptr getMenuItem();


    /**
     * Gets a PopupEditorComponent configured to edit this button's data.
     *
     * @param onConfirm  A callback function that will run if changes are made 
     *                   and confirmed in the new editor.  This should be used
     *                   for updating button properties that aren't managed 
     *                   internally (e.g., button icon, but not button bounds).
     *
     * @return  A new PopupEditorComponent, ready to be added to the screen.
     */
    AppMenuPopupEditor* getEditor
    (const std::function<void(AppMenuPopupEditor*) >& onConfirm);

    /**
     * Calling this method will create a message box asking for user 
     * confirmation that this button and its source should be removed.
     * If the user clicks "OK", removeButtonSource is called.
     *
     * @param onRemove  A callback function that is responsible for removing
     *                  his button from its parent if the user clicks "OK".
     */
    void confirmRemoveButtonSource(const std::function<void() >& onRemove);

    /**
     * If possible, change the index of this button's data source by some
     * offset amount.
     *
     * @param offset  This will be added to the button's current index, if 
     *                possible.
     *
     * @return  True iff the operation succeeded.
     */
    virtual bool moveDataIndex(int offset);

    /**
     * Checks if this button is currently selected.
     * 
     * @return  True iff this button is selected.
     */
    bool isSelected() const;

    /**
     * Select or un-select this button.
     *
     * @param select   Sets the button as selected if true and unselected if
     *                 false.
     */
    void setSelected(bool select);
protected:

    /**
     * Triggers whenever the button is selected or unselected.
     */
    virtual void selectionStateChanged() { }

    /**
     * Requests an icon from the icon thread.
     *
     * @param icon   An icon's full path, or the name of an icon file located
     *               in common icon directories.
     */
    void loadIcon(juce::String icon);


    /**
     * Reload this button's data from its menu item.
     */
    virtual void reloadDataFromSource();

    /**
     * Gets the button's text bounds.
     *
     * @return   The area relative to this button's position where
     *           it will draw its name.
     */
    const juce::Rectangle<float>& getTextBounds() const;

    /**
     * Gets the button's image bounds.
     *
     * @return   The area relative to this button's position where
     *           it will draw its image.
     */
    const juce::Rectangle<float>& getImageBounds() const;

    /**
     * Gets the button's title font.
     *
     * @return  The font used to draw this button's title.
     */
    const juce::Font& getTitleFont() const;

    /**
     * Set new bounds to draw the button title within.
     *
     * @param textBounds  The area relative to this button's position where
     *                    it will draw its name.
     */
    void setTextBounds(const juce::Rectangle<float>& bounds);

    /**
     * Set new bounds to draw the button icon within.
     *
     * @param bounds  The area relative to this button's position where
     *                it will draw its icon.
     */
    void setImageBounds(const juce::Rectangle<float>& bounds);

    /**
     * Sets if this button will draw an outline around its border.
     *
     * @param shouldDraw   True iff the outline should be drawn.
     */
    void setDrawBorder(bool shouldDraw);

    /**
     * Sets if this button will fill in its background with its background
     * color.
     * 
     * @param shouldFill  True to fill in the entire background, false to just
     *                    fill in the area behind the button text.
     */
    void setFillBackground(bool shouldFill);


    /**
     * Sets the button's title font.
     *
     * @param font   The font used to draw this button's title.
     */
    void setTitleFont(const juce::Font& font);

    /**
     * Set the text justification of the button title.
     *
     * @param justification  This will be used to position button title within
     *                       the text bounds.
     */
    void setTextJustification(juce::Justification justification);

private:
    /**
     * Custom button painting method to be called by juce library code.
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

    //Icon image to draw
    juce::Image appIcon;

    //bounds used to position and scale the button title and icon
    juce::Rectangle<float> textBounds;
    juce::Rectangle<float> imageBounds;
    
    //title font and justification
    juce::Font titleFont;
    juce::Justification textJustification = juce::Justification::centredLeft;
    
    //Pre-calculated text width
    int textWidth;
    //background draw options
    bool fillBackground = true;
    bool drawBorder = true;
    
    //Menu item data object
    AppMenuItem::Ptr menuItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuButton)
};
