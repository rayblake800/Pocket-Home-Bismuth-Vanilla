/**
 * @file AppMenuFolder.h 
 * 
 * AppMenuFolder components organize and manage one folder in an
 * AppMenuComponent.
 */

#pragma once
#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../GridLayoutManager.h"
#include "Menu Items/AppMenuItem.h"
#include "AppMenuButton.h"

class AppMenuFolder : public Component {
public:
    /**
     * Creates a new folder component, loading menu buttons from a
     * folder menu item.
     * 
     * @param folderItem 
     * @param btnListener will be assigned to listen to all folder
     * menu buttons.
     * @param buttonNameMap stores all menu buttons and re-uses them
     * when possible.
     * @param iconThread loads icons for new menu buttons.
     */
    AppMenuFolder(AppMenuItem::Ptr folderItem, MouseListener* btnListener,
            std::map<String, AppMenuButton::Ptr>& buttonNameMap,
            IconThread& iconThread);

    virtual ~AppMenuFolder();

    /**
     * Create an AppMenuButton component for an AppMenuItem.
     * @param menuItem
     */
    virtual AppMenuButton::Ptr createMenuButton
    (AppMenuItem::Ptr menuItem) = 0;


    /**
     * Sets the button grid row and column sizes, updating button layout
     * if the values change
     */
    void updateGridSize(int maxRows, int maxColumns);

    /**
     * Reload all folder menu buttons from their source menu item.
     */
    void reload();

    /**
     * @return number of menu buttons in the folder.
     */
    int size();

    /**
     * Find the index of a menu button in this folder
     * 
     * @param menuButton
     * @return the button's index, or -1 if the button is not in this
     * folder.
     */
    int getButtonIndex(AppMenuButton::Ptr menuButton);

    /**
     * @return The selected menu button in this folder.
     * This will only return nullptr if there are no buttons in 
     * this folder.
     */
    AppMenuButton::Ptr getSelectedButton();

    /**
     * Set this folder's selected menu button
     * @param index
     * @return true if index was valid and the selection changed.
     */
    bool selectIndex(int index);

    /**
     * Deselects the selected button, if one exists
     */
    void deselect();

    /**
     * @return the index of the selected menu button, or -1 if no button
     * is selected
     */
    int getSelectedIndex();

    /**
     * Creates or reloads a button for a menu item, inserting it into
     * the folder at a specific index. This shifts forward any buttons at 
     * indices equal or greater than the index. 
     * 
     * @param newItem
     * @param index should be between 0 and appFolder.size(),
     * inclusive.  Values outside of this range will be rounded to
     * the nearest valid value.
     * @param updateLayout if set to true, the button layout will be
     * reloaded after the new button is inserted.  If inserting many buttons,
     * it's better to set this to false and update the layout once at the
     * end.
     */
    void insertButton(AppMenuItem::Ptr newItem, int index,
            bool updateLayout = true);

    /**
     * Remove the button at a given index, shifting back any buttons
     * at greater indices to fill the gap.
     * 
     * @param index should be between 0 and appFolder.size(), 
     * inclusive, otherwise this method will do nothing.
     */
    void removeButton(int index);

    /**
     * Swap the indices and positions of two buttons in the folder.
     * Both indices must be valid, or nothing will happen.
     * @param btnIndex1
     * @param btnIndex2
     */
    void swapButtons(int btnIndex1, int btnIndex2);

    /**
     * Set the relative placement of folder buttons within the folder.
     * 
     * @param margin space between components and the edge of the
     * folder component, as a fraction of folder width.
     */
    void setMargin(float margin);

    /**
     * Set the relative space between folder buttons.
     * 
     * @param margin space between components and the edge of the
     * folder component, as a fraction of folder width.
     * 
     * @param xPadding horizontal space between folder child
     * components, as a fraction of folder width.
     * 
     * @param yPadding vertical space between folder child
     * components, as a fraction of folder height.
     */
    void setPadding(float xPadding, float yPadding);

    /**
     * @return margin space between components and the edge of the
     * folder component, as a fraction of folder width.
     */
    float getMargin();

    /**
     * @return horizontal space between folder child
     * components, as a fraction of folder width.
     */
    float getXPadding();

    /**
     * @return vertical space between folder child
     * components, as a fraction of folder height.
     */
    float getYPadding();

    /**
     * Clear folderLayout,remove all child components, reload the
     * button layout, and re-add the layout buttons as child
     * components.
     */
    virtual void layoutButtons();

protected:
    /**
     * Reposition folder buttons when folder bounds change.
     */
    void resized() override;


    //New buttons will need this to load their icons.
    IconThread& iconThread;

    int maxRows = 1;
    int maxColumns = 1;
    int buttonsPerPage = 1;
private:

    /**
     * Given a list of folder buttons, return an appropriate layout
     * for positioning them in the folder component.
     * 
     * @param buttons
     * @return a Layout containing all items in the button array.
     */
    virtual GridLayoutManager::Layout buildFolderLayout
    (Array<AppMenuButton::Ptr>& buttons) = 0;

    /**
     * @param index
     * @return true for any valid button index
     */
    inline bool validBtnIndex(int index) {
        return index >= 0 && index < folderButtons.size();
    }

    //Listener to assign to all button components
    MouseListener* btnListener = nullptr;
    //Reference to the AppMenuComponent's button map
    std::map<String, AppMenuButton::Ptr>& buttonNameMap;
    //Holds the folder menu item used to load this folder's menu items
    AppMenuItem::Ptr sourceFolderItem = nullptr;
    //folder layout manager and relative spacing values.
    GridLayoutManager folderLayout;
    float margin = 0;
    float xPadding = 0;
    float yPadding = 0;
    //Holds all menu buttons
    Array<AppMenuButton::Ptr> folderButtons;
    //Tracks selected button index
    int selectedIndex = -1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuFolder)
};