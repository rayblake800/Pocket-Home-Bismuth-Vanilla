/* 
 * @file AppMenuButton.h
 * 
 * AppMenuButton is a custom button type for launching applications and 
 * opening application directories. Each button displays an icon and a title,
 * and may have category strings or a command string stored. Button actions
 * are handled elsewhere, using this data.
 * @see AppMenuComponent
 */

#pragma once
#include "../../Configuration/Configurables/ConfigurableComponent.h"
#include "../IconThread.h"
#include "../Popup Editor Components/AppMenuPopupEditor.h"

class AppMenuButton : public Button, public ReferenceCountedObject,
public ConfigurableComponent {
public:
    typedef ReferenceCountedObjectPtr<AppMenuButton> Ptr;

    /**
     * Create a new AppMenuButton
     * @param name sets the button display name
     * @param index records the button position in its column
     * @param column records the button column in the AppMenu
     * @param iconThread is needed to load the button icon.
     */
    AppMenuButton(String name, int index, int column, IconThread& iconThread);

    /**
     * Set whether this button is currently selected.
     * @param select sets the new selection state
     */
    void setSelected(bool select);

    /**
     * @return true if this button is for an application folder
     */
    virtual bool isFolder() const = 0;

    /**
     * @return the display name of the associated application
     */
    virtual String getAppName() const = 0;

    /**
     * @return the application shell command or directory path.
     */
    virtual String getCommand() const = 0;

    /**
     * @return all application categories linked to this button.
     */
    virtual Array<String> getCategories() const = 0;

    /**
     * @return the name or path used to load the icon file. 
     */
    virtual String getIconName() const = 0;

    /**
     * @return the icon image used by this button.
     */
    const Image& getIcon();

    /**
     * @return the button position in its column
     */
    int getIndex() const;

    /**
     * @return the button column in the AppMenu
     */
    int getColumn() const;

    /**
     * Updates the stored button index.  Note that this doesn't move the
     * button, just updates the value that identifies where it is.
     * @param index a new index in the column to assign to this button.
     */
    void setIndex(int index);

    /**
     * 
     * Updates the stored button column.  Note that this doesn't move the
     * button, just updates the value that identifies where it is.
     * @param column a new column number to assign to this button.
     */
    void setColumn(int column);

    /**
     * Gets a PopupEditorComponent configured to edit this button
     * @return a new PopupEditorComponent, ready to be added to the screen.
     */
    virtual AppMenuPopupEditor* getEditor() = 0;

    /**
     * Calling this method will create a message box asking for user 
     * confirmation that this button and its source should be removed.
     * If the user clicks "OK", removeButtonSource is called.
     */
    void confirmRemoveButtonSource();

    /**
     * Removes the source of this button's data, and remove the button
     * from its parent component.
     */
    virtual void removeButtonSource() = 0;

    /**
     * Return true if this button's data source has an index that can be
     * moved by a given amount.
     * @param offset some value to add to the button index
     * @return true if this button's data source has an index value i that can
     * be changed to i+offset 
     */
    virtual bool canChangeIndex(int offset) = 0;

    /**
     * If possible, change the index of this button's data source by some
     * offset amount.
     * @param offset will be added to the button's current index, if possible.
     */
    virtual void moveDataIndex(int offset) = 0;

    /**
     * @return the size of an AppMenuButton with the current window size
     * and configuration file ratios.
     */
    static Rectangle<int> getButtonSize();

    /**
     * Sets a callback to run when button data changes and should be
     * reloaded.  Only one component should be managing AppMenuButtons at
     * any one time, so this only needs to be called once when that function
     * initializes.
     * @param reload should be set by the component that is managing
     * AppMenuButtons to re-calculate button data and arrangement.
     */
    static void setReloadButtonsCallback(std::function<void() > reload);
protected:
    /**
     * Custom button painting method.
     */
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown);

    /**
     * Re-calculates draw values whenever the button is resized
     */
    void resized() override;

    /**
     * Load button colors from configuration files.
     * @param assetNames is ignored, this component has no configurable
     * image asset.
     * @param colours
     */
    virtual void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours);

    /**
     * Requests an icon from the icon thread.
     * @param icon an icon's full path, or the name of an icon file located
     * in common icon directories.
     */
    void loadIcon(String icon);

    //Title to display on the dialog box shown when confirming button deletion
    String confirmDeleteTitle;
    //Message to display on the dialog box shown when confirming button deletion
    String confirmDeleteMessage;
    //Icon image to draw
    Image appIcon;
    //Object used to load icons
    IconThread& iconThread;
    //Callback function for reloading all AppMenuButtons
    static std::function<void() > reloadAllButtons;

private:
    //bounds for drawing app/folder name
    Rectangle<float> textBox;
    //bounds for drawing button icon
    Rectangle<float> imageBox;
    //font for drawing app/folder name
    Font titleFont;
    //true iff this button is selected
    bool selected = false;
    //index in the column, starting at the top
    int index;
    //containing column number, counted left to right
    int column;
    //Button text color
    Colour textColour;
    //Button background fill color, if not selected.
    Colour fillColour;
    //Button background fill color, if selected.
    Colour selectedFillColour;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuButton)
};
