/* 
 * @file AppMenuButton.h
 * 
 * AppMenuButton is a custom button type for launching applications and 
 * opening application directories. Each button displays an icon and a title.
 * Each button has an associated AppMenuItem that can be used for accessing and 
 * modifying the button's menu data.  
 * @see AppMenuComponent, AppMenuItem
 */

#pragma once
#include "../../../Configuration/Configurables/ConfigurableComponent.h"
#include "../IconThread.h"
#include "../Popup Editor Components/AppMenuPopupEditor.h"
#include "AppMenuItem.h"

class AppMenuButton : public Button, public ReferenceCountedObject,
public ConfigurableComponent {
public:
    typedef ReferenceCountedObjectPtr<AppMenuButton> Ptr;

    enum ColourIds {
        textColourId = 0x1900200,
        backgroundColourId = 0x1900201,
        selectionColourId = 0x1900202,
        borderColourId = 0x1900203
    };

    /**
     * Create a new AppMenuButton
     * @param menuItem sets the button's data source
     * @param iconThread is needed to load the button icon.
     * @param name sets the button's internal component name
     */
    AppMenuButton(AppMenuItem::Ptr menuItem, IconThread& iconThread,
            String name = String::empty);

    virtual ~AppMenuButton();

    /**
     * Get this button's menu data.
     * @return A pointer to this button's internal AppMenuItem.  This pointer
     * will be valid as long as this AppMenuButton still exists.
     */
    AppMenuItem::Ptr getMenuItem();


    /**
     * Gets a PopupEditorComponent configured to edit this button's data
     * @param onConfirm callback function that will run if changes are made and
     * confirmed in the new editor.  This should be used for updating button
     * properties that aren't managed internally, such as button position.
     * @return a new PopupEditorComponent, ready to be added to the screen.
     */
    AppMenuPopupEditor* getEditor
    (const std::function<void(AppMenuPopupEditor*) >& onConfirm);

    /**
     * Calling this method will create a message box asking for user 
     * confirmation that this button and its source should be removed.
     * If the user clicks "OK", removeButtonSource is called.
     * @param onRemove callback function that is responsible for removing this
     * button from its parent if the user clicks "OK"
     */
    void confirmRemoveButtonSource(const std::function<void() >& onRemove);

    /**
     * If possible, change the index of this button's data source by some
     * offset amount.
     * @param offset will be added to the button's current index, if possible.
     * @return true if the operation succeeded.
     */
    virtual bool moveDataIndex(int offset);

    /**
     * @return true if this button is selected, false otherwise.
     */
    bool isSelected() const;

    /**
     * @param select sets the button as selected if true and unselected if
     * false.
     */
    void setSelected(bool select);
protected:

    /**
     * Triggers whenever the button is selected or unselected
     */
    virtual void selectionStateChanged() {
    }

    /**
     * Requests an icon from the icon thread.
     * @param icon an icon's full path, or the name of an icon file located
     * in common icon directories.
     */
    void loadIcon(String icon);


    /**
     * Reload this button's data from its menu item
     */
    virtual void reloadDataFromSource();
  
    /**
     * @return  the area relative to this button's position where
     * it will draw its name
     */
    const Rectangle<float>& getTextBounds() const;

    /**
     * @return  the area relative to this button's position where
     * it will draw its image
     */
    const Rectangle<float>& getImageBounds() const;
    
    /**
     * @return the font used to draw this button's title.
     */
    const Font& getTitleFont() const;
    
    
    /**
     * @param textBounds the area relative to this button's position where
     * it will draw its name
     */
    void setTextBounds(const Rectangle<float>& bounds);

    /**
     * @param bounds the area relative to this button's position where
     * it will draw its image
     */
    void setImageBounds(const Rectangle<float>& bounds);
    
    /**
     * Sets if this button will draw its border
     * @param shouldDraw
     */
    void setDrawBorder(bool shouldDraw);
    
    /**
     * Sets if this button will fill in its background with its background
     * color.
     * @param shouldFill
     */
    void setFillBackground(bool shouldFill);

    
    /**
     * @param font will be used to draw this button's title.
     */
    void setTitleFont(const Font& font);
    
    /**
     * @param justification will be used to position button text within
     * text bounds.
     */
    void setTextJustification(Justification justification);

private:
    
    //Icon image to draw
    Image appIcon;
    //Object used to load icons
    IconThread& iconThread;

    Rectangle<float> textBounds;
    Rectangle<float> imageBounds;
    Font titleFont;
    Justification textJustification = Justification::centredLeft;
    bool fillBackground = true;
    bool drawBorder = true;

    /**
     * 
     * @param g
     * @param isMouseOverButton
     * @param isButtonDown
     */
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown);
    //Menu item data object
    AppMenuItem::Ptr menuItem;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuButton)
};
