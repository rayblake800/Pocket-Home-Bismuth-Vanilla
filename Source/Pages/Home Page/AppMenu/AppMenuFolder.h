/**
 * @file AppMenuFolder.h 
 * 
 * AppMenuFolder components organize and manage one folder in an
 * AppMenuComponent.
 */

#pragma once
#include "JuceHeader.h"
#include "RelativeLayoutManager.h"
#include "AppMenuItem.h"
#include "AppMenuButton.h"

class AppMenuFolder : public Component {
public:
    /**
     * Creates a new folder component, loading menu buttons from a
     * folder menu item.
     * 
     * @param folderItem     Provides all menu items for this folder.
     *
     * @param btnListener    This will be assigned to listen to all folder menu 
     *                        buttons.
     *
     * @param buttonNameMap  This stores all menu buttons and re-uses them
     *                        when possible.
     *                       
     * @param iconThread     Loads icons for new menu buttons.
     */
    AppMenuFolder(
            AppMenuItem::Ptr folderItem, 
            MouseListener* btnListener,
            std::map<String, AppMenuButton::Ptr>& buttonNameMap,
            IconThread& iconThread);

    virtual ~AppMenuFolder() { }

    /**
     * Create an AppMenuButton component for an AppMenuItem.
     * 
     * @param menuItem    The menu item to assign to the new button.
     *
     * @param iconThread  The shared icon cache used to find icon files.
     */
    virtual AppMenuButton::Ptr createMenuButton
    (AppMenuItem::Ptr menuItem, IconThread& iconThread) = 0;

    /**
     * Sets the button grid row and column sizes, updating button layout
     * if the values change.
     *
     * @param maxRows     Maximum number of menu item rows to display on screen.
     *
     * @param maxColumns  Maximum number of menu item columns to display on
     *                     screen.
     */
    void updateGridSize(int maxRows, int maxColumns);

    /**
     * Reload all folder menu buttons from their source menu item.
     */
    void reload();


    /**
     * Sets this folder's selected menu button.
     *
     * @param index  The menu button index to select.
     *
     * @return true iff index was valid and the selection changed.
     */
    bool selectIndex(int index);

    /**
     * Deselects the selected button, if one exists.
     */
    void deselect();

    /**
     * Creates or reloads a button for a menu item, inserting it into
     * the folder at a specific index. This shifts forward any buttons at 
     * indices equal or greater than the index. 
     * 
     * @param newItem  The menu item to add to a new button or find in a cached
     *                 button.
     *
     * @param index         The new button's index, which should be between 0 
     *                       and appFolder.size(), inclusive. Values outside of 
     *                       this range will be rounded to the nearest valid 
     *                       value.
     *
     * @param updateLayout  If set to true, the button layout will be
     *                       reloaded after the new button is inserted.  If 
     *                       inserting many buttons, it's better to set this to 
     *                       false and update the layout once at the end.
     */
    void insertButton(AppMenuItem::Ptr newItem, int index,
            bool updateLayout = true);

    /**
     * Remove the button at a given index, shifting back any buttons
     * at greater indices to fill the gap.
     * 
     * @param index   The index to remove. This should be between 0 and 
     *                 appFolder.size(),  inclusive, otherwise this method will 
     *                 do nothing.
     */
    void removeButton(int index);

    /**
     * Swap the indices and positions of two buttons in the folder.
     * Both indices must be valid, or nothing will happen.
     *
     * @param btnIndex1  First button index.
     *
     * @param btnIndex2  Second button index.
     */
    void swapButtons(int btnIndex1, int btnIndex2);

    /**
     * Set the relative placement of folder buttons within the folder.
     * 
     * @param margin  The space between components and the edge of the
     *                 folder component, as a fraction of folder width.
     */
    void setMargin(float margin);

    /**
     * Set the relative space between folder buttons.
     * 
     * @param margin   The space between button components and the edge of the
     *                  folder component, as a fraction of folder width.
     * 
     * @param xPadding The horizontal space between folder child components, 
     *                  as a fraction of folder width.
     * 
     * @param yPadding The vertical space between folder child components, as 
     *                  a fraction of folder height.
     */
    void setPadding(float xPadding, float yPadding);


    /**
     * Clear folderLayout,remove all child components, reload the
     * button layout, and re-add the layout buttons as child
     * components at their new positions.
     */
    void layoutButtons();

    /**
     * @return number of menu buttons in the folder.
     */
    int getButtonCount() const;

    /**
     * Find the index of a menu button in this folder.
     * 
     * @param menuButton  A button to search for in the folder.
     * 
     * @return the button's index, or -1 if the button is not in this
     *          folder.
     */
    int getButtonIndex(AppMenuButton::Ptr menuButton) const;

    /**
     * Get the display name of a menu button.
     * 
     * @param index  
     * 
     * @return the button title at this index, or the empty string
     *          if index does not correspond to a menu button.
     */
    String getMenuButtonName(int index) const;

    /**
     * @return the index of the selected menu button, or -1 if no button
     *          is selected.
     */
    int getSelectedIndex() const;

    /**
     * @return the selected button in this folder, or nullptr if there
     *          is no selected button.
     */
    AppMenuButton::Ptr getSelectedButton();


    /**
     * @return margin space between components and the edge of the
     *          folder component, as a fraction of folder width.
     */
    float getMargin() const;

    /**
     * @return horizontal space between folder child  components, as a 
     *          fraction of folder width.
     */
    float getXPadding() const;

    /**
     * @return vertical space between folder child components, as a fraction of
     *          folder height.
     */
    float getYPadding() const;

    /**
     * @return the minimum width, in pixels, needed by this folder to display 
     *          its contents properly. By default this returns 0.
     */
    virtual int getMinimumWidth();

protected:
    /**
     * Reposition folder buttons when folder bounds change.
     */
    void resized() override;

    /**
     * @param index
     * 
     * @return the title of the menu button at this index, or String()
     *          if there is no button at this index.
     */
    String getButtonTitle(int index);
    
    /**
     * @return the maximum number of menu item rows to show on screen.
     */
    int getMaxRows() const;

    /**
     * @return the maximum number of menu item columns to show on screen.
     */
    int getMaxColumns() const;

    
    /**
     * @return the maximum number of menu items to show on screen.
     */
    inline int getButtonsPerPage() const {
        return getMaxRows() * getMaxColumns();
    }
private:

    /**
     * Given a list of folder buttons, return an appropriate layout
     * for positioning them in the folder component.
     * 
     * @param buttons
     * 
     * @return a Layout containing all items in the button array.
     */
    virtual RelativeLayoutManager::Layout buildFolderLayout
    (Array<AppMenuButton::Ptr>& buttons) = 0;

    /**
     * Checks if a number is a valid button index.
     *
     * @param index
     *
     * @return true  iff the folder has a menu button with this index.
     */
    inline bool validBtnIndex(int index) {
        return index >= 0 && index < folderButtons.size();
    }

    //New buttons will need this to load their icons.
    IconThread& iconThread;
    //Listener to assign to all button components
    MouseListener* btnListener = nullptr;
    //Reference to the AppMenuComponent's button map
    std::map<String, AppMenuButton::Ptr>& buttonNameMap;
    //Holds the folder menu item used to load this folder's menu items
    AppMenuItem::Ptr sourceFolderItem = nullptr;
    //folder layout manager and relative spacing values.
    RelativeLayoutManager folderLayout;
    float margin = 0;
    float xPadding = 0;
    float yPadding = 0;
    int maxRows = 1;
    int maxColumns = 1;
    //Holds all menu buttons
    Array<AppMenuButton::Ptr> folderButtons;
    //Tracks selected button index
    int selectedIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuFolder)
};
