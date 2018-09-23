#pragma once
#include "JuceHeader.h"
#include "LayoutManager.h"
#include "AppMenuItem.h"
#include "AppMenuButton.h"

/**
 * @file  AppMenuFolder.h 
 * 
 * @brief  Displays, organizes and manages one folder in an AppMenuComponent.
 * 
 * AppMenuFolder components take a set of AppMenuItems, create AppMenuButton
 * components for each of them, and positions those components on the screen.
 * The AppMenuFolder allows the user to select a single button in the menu,
 * edit menu items, add new items to the folder, or remove items from the 
 * folder.  AppMenuFolder also provides methods for setting menu dimensions,
 * margin, and padding.
 *
 * AppMenuFolder is abstract, allowing for multiple alternate folder types.
 * AppMenuFolder subclasses are responsible for constructing AppManuButton
 * component objects from AppMenuItem objects, and for arranging those buttons
 * within the folder layout.
 */

class AppMenuFolder : public juce::Component
{
public:
    /**
     * @brief  Creates a new folder component, loading menu buttons from a
     *         folder menu item.
     * 
     * @param folderItem     Provides all menu items for this folder.
     *
     * @param btnListener    Listens to all folder menu buttons.
     *
     * @param buttonNameMap  Stores all menu buttons so they can be re-used.
     */
    AppMenuFolder(
            const AppMenuItem::Ptr folderItem,
            const MouseListener* btnListener,
            std::map<juce::String, AppMenuButton::Ptr>& buttonNameMap);

    virtual ~AppMenuFolder() { }

    /**
     * @brief  Creates an AppMenuButton component for an AppMenuItem.
     * 
     * @param menuItem  A pointer to the menu item to assign to the new button.
     *
     * @return          A reference-counted pointer to the new button component.
     */
    virtual AppMenuButton::Ptr createMenuButton
    (const AppMenuItem::Ptr menuItem) = 0;

    /**
     * @brief  Sets the button grid row and column sizes, updating button layout
     *         if the values change.
     *
     * @param maxRows     Maximum number of menu item rows to display on screen.
     *
     * @param maxColumns  Maximum number of menu item columns to display on
     *                    screen.
     */
    void updateGridSize(const int maxRows, const int maxColumns);

    /**
     * @brief  Reloads all folder menu buttons from their source menu item.
     */
    void reload();

    /**
     * @brief  Sets this folder's selected menu button.
     *
     * @param index  The menu button index to select.
     *
     * @return       True if the selection changed, false if the index was
     *               invalid or matched the current selection and no change
     *               occurred.
     */
    bool selectIndex(const int index);

    /**
     * @brief  Deselects the selected button, if one exists.
     */
    void deselect();

    /**
     * @brief  Creates or reloads a button for a menu item, inserting it into
     *         the folder at a specific index. 
     *
     * This shifts forward any buttons at indices equal or greater than the 
     * insertion index. 
     * 
     * @param newItem        The menu item used to find a cached button or 
     *                       create a newbutton.
     *
     * @param index          The new button's index, which should be between 0 
     *                       and appFolder.size(), inclusive. Values outside of 
     *                       this range will be rounded to the nearest valid 
     *                       value.
     *
     * @param updateLayout   If set to true, the button layout will be
     *                       reloaded after the new button is inserted.  If 
     *                       inserting many buttons, it's better to set this to 
     *                       false and update the layout once at the end.
     */
    void insertButton(const AppMenuItem::Ptr newItem, const int index,
            const bool updateLayout = true);

    /**
     * @brief  Removes the button at a given index, shifting back any buttons
     *         at greater indices to fill the gap.
     * 
     * @param index   The index to remove. This should be between 0 and 
     *                appFolder.size(),  inclusive, otherwise this method will 
     *                do nothing.
     */
    void removeButton(const int index);

    /**
     * @brief  Swaps the indices and positions of two buttons in the folder.
     * 
     * Both indices must be valid, or nothing will happen.
     *
     * @param btnIndex1  First button index.
     *
     * @param btnIndex2  Second button index.
     */
    void swapButtons(const int btnIndex1, const int btnIndex2);

    /**
     * @brief  Sets the relative size of the folder's margins.
     * 
     * @param margin  The space between button components and the edge of the
     *                folder component, as a fraction of folder width.
     */
    void setMargin(const float margin);

    /**
     * @brief  Sets the relative space between folder buttons.
     * 
     * @param xPadding The horizontal space between folder child components, 
     *                 as a fraction of folder width.
     * 
     * @param yPadding The vertical space between folder child components, as 
     *                 a fraction of folder height.
     */
    void setPadding(const float xPadding, const float yPadding);


    /**
     * @brief  Updates the layout of all menu buttons within the folder.
     *
     * This clears the folder layout, removes all child components, reloads
     * the button layout, and re-adds the layout buttons as child components at
     * their new positions.
     */
    void layoutButtons();

    /**
     * @brief  Gets the number of menu buttons in the folder.
     *
     * @return  The number of AppMenuButton child components.
     */
    int getButtonCount() const;

    /**
     * @brief  Finds the index of a menu button in this folder.
     * 
     * @param menuButton  A button to search for in the folder.
     * 
     * @return            The button's index, or -1 if the button is not in this
     *                    folder.
     */
    int getButtonIndex(const AppMenuButton::Ptr menuButton) const;

    /**
     * @brief  Gets the display name of a menu button.
     * 
     * @param index  The index of a menu item in the folder. 
     * 
     * @return       The button title at this index, or the empty string
     *               if the index does not correspond to a menu button.
     */
    juce::String getMenuButtonName(const int index) const;

    /**
     * @brief  Gets the currently selected button's index in the folder.
     *
     * @return  The index of the selected menu button, or -1 if no button
     *          is selected.
     */
    int getSelectedIndex() const;

    /**
     * @brief  Gets the folder's selected menu button.
     *
     * @return   A pointer tothe selected button in this folder, or nullptr if
     *           there is no selected button.
     */
    AppMenuButton::Ptr getSelectedButton();

    /**
     * @brief  Gets the folder's  relative margin size.
     *
     * @return  The margin space between components and the edge of the
     *          folder component, as a fraction of folder width.
     */
    float getMargin() const;

    /**
     * @brief  Gets the folder's relative horizontal padding size.
     *
     * @return  The amount of horizontal space between folder child  components,
     *          as a fraction of folder width.
     */
    float getXPadding() const;

    /**
     * @brief  Gets the folder's relative vertical padding size.
     *
     * @return  The amount ofvertical space between folder child components, as
     *          a fraction of folder height.
     */
    float getYPadding() const;

    /**
     * @brief  Gets the folder's minimum width.
     *
     * @return   The minimum width, in pixels, needed by this folder to display 
     *           its contents properly. By default this returns 0.
     */
    virtual int getMinimumWidth();

protected:
    /**
     * @brief  Repositions folder buttons when folder bounds change.
     */
    void resized() override;

    /**
     * @brief  Gets the display title of a folder menu item.
     *
     * @param index  The index of a menu item in this folder.
     * 
     * @return       The title of the menu button at this index, or the empty
     *               string if there is no button at this index.
     */
    juce::String getButtonTitle(const int index);

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
    inline int getButtonsPerPage() const
    {
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
    virtual LayoutManager::Layout buildFolderLayout
    (juce::Array<AppMenuButton::Ptr>& buttons) = 0;

    /**
     * Checks if a number is a valid button index.
     *
     * @param index
     *
     * @return true  iff the folder has a menu button with this index.
     */
    inline bool validBtnIndex(int index)
    {
        return index >= 0 && index < folderButtons.size();
    }

    //Listener to assign to all button components
    juce::MouseListener* btnListener = nullptr;
    //Reference to the AppMenuComponent's button map
    std::map<juce::String, AppMenuButton::Ptr>& buttonNameMap;
    //Holds the folder menu item used to load this folder's menu items
    AppMenuItem::Ptr sourceFolderItem = nullptr;
    //folder layout manager and relative spacing values.
    LayoutManager folderLayout;
    float margin = 0;
    float xPadding = 0;
    float yPadding = 0;
    int maxRows = 1;
    int maxColumns = 1;
    //Holds all menu buttons
    juce::Array<AppMenuButton::Ptr> folderButtons;
    //Tracks selected button index
    int selectedIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuFolder)
};
