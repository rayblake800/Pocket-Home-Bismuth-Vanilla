#pragma once
#include "JuceHeader.h"

/**
 * @file  MenuItemData.h
 *
 * @brief  Reads and writes properties of a menu item. 
 */
class MenuItemData : public juce::ReferenceCountedObject
{
public:
    
    /* Custom reference-counting pointer object type. */
    typedef juce::ReferenceCountedObjectPtr<MenuItemData> Ptr;

    /**
     * @brief  Creates a menu data object for an item in the application menu.
     *
     * @param parent  The folder menu item this menu item is located in within
     *                the menu tree, or nullptr if this menu item is the root
     *                folder menu item.
     *
     * @param index   The object's index within its parent folder, or -1 if
     *                this menu item is the root folder menu item.
     */
    MenuItemData(const MenuItemData::Ptr parent, const int index);
    
    virtual ~MenuItemData() { }

    /**
     * @brief  Gets the menu item's displayed title.
     *
     * @return  The displayed title string.
     */
    virtual juce::String getTitle() const = 0;

    /**
     * @brief  Gets the name or path use to load the menu item's icon file.
     *
     * @return  The name or path of the icon.
     */
    virtual juce::String getIconName() const = 0;

    /**
     * @brief  Gets the menu item's application launch command.
     *
     * @return  The launch command string, or the empty string if the menu item
     *          does not launch an application.
     */
    virtual juce::String getCommand() const = 0;

    /**
     * @brief  Checks if this menu item launches an application in a new
     *         terminal window.
     *
     * @return  Whether the menu item has a launch command it should run in a 
     *          new terminal window
     */
    virtual bool getLaunchedInTerm() const = 0;

    /**
     * @brief  Gets the application categories connected to this menu item.
     *
     * @return  Any category strings assigned to this menu item.
     */
    virtual juce::StringArray getCategories() const = 0;

    /**
     * @brief  Sets the menu item's displayed title.
     *
     * @param title  The new title string to display.
     */
    virtual void setTitle(const juce::String& title) = 0;

    /**
     * @brief  Sets the name or path used to load the menu item's icon file.
     *
     * @param iconName  The new icon name or path.
     */
    virtual void setIconName(const juce::String& iconName) = 0;

    /**
     * @brief  Sets the menu item's application launch command.
     *
     * @param newCommand  The new command string to run when this menu item is
     *                    clicked.
     */
    virtual void setCommand(const juce::String& newCommand) = 0;

    /**
     * @brief  Sets if this menu item runs its command in a new terminal window.
     *
     * @param termLaunch  True to run any launch command assigned to this
     *                    menu item within a new terminal window.
     */
    virtual void setLaunchedInTerm(const bool termLaunch) = 0;

    /**
     * @brief  Sets the application categories connected to this menu item.
     *
     * @param categories  The new set of category strings to assign to this menu
     *                    item.
     */
    virtual void setCategories(const juce::StringArray& categories) = 0;
    
    /**
     * @brief  Gets this menu item's parent folder.
     *
     * @return  The parent folder's data, or nullptr if this menu item is the
     *          root folder menu item.
     */
    MenuItemData::Ptr getParentFolder() const;

    /**
     * @brief  Gets this menu item's index within its parent folder.
     *
     * @return  The index, or -1 if this menu item is the root folder menu item. 
     */
    int getIndex() const;

    /**
     * @brief  Checks if this menu item represents a folder within the menu.
     *
     * @return  Whether this menu item opens a new menu folder.
     */
    bool isFolder() const;

    /**
     * @brief  Gets the number of folder items held by this menu item.
     *
     * @return  The number of folder items this menu item holds, or zero if this
     *          menu item is not a folder.
     */
    int getFolderSize() const;

    /**
     * @brief  Deletes this menu item data from its source.
     */
    virtual void deleteFromSource() = 0;

    /**
     * @brief  Writes all changes to this menu item back to its data source.
     */
    virtual void updateSource() = 0;

    /**
     * @brief  Gets the number of folder items held by this menu item that can
     *         be reordered.
     *
     * Movable child folder items always come before un-movable ones, so any
     * child folder items with an index less than the movable child count can
     * have their positions swapped.
     *
     * @return  The number of child folder items held that can be re-arranged
     *          in any order.
     */
    virtual int getMovableChildCount() = 0;

    MenuItemData::Ptr getChild(const int index) const;
    juce::Array<MenuItemData::Ptr> getChildren() const;

    bool addChild(const MenuItemData::Ptr newChild);
    bool insertChild(const MenuItemData::Ptr newChild, const int index);
    bool replaceChild(const MenuItemData::Ptr newChild, const int index);
    bool removeChild(const int index);
    bool swapChildren(const int childIdx1, const int childIdx2);
    
    virtual void saveChanges() = 0;
    virtual void removeFromSource() = 0;
    
    /**
     * @brief  Gets an appropriate title to use for a deletion confirmation 
     *         window.
     *
     * @return  A localized confirmation title string.
     */
    virtual juce::String getConfirmDeleteTitle() const = 0;

    /**
     * @brief  Gets appropriate descriptive text for a deletion confirmation 
     *         window.
     *
     * @return  A localized confirmation description string.
     */
    virtual juce::String getConfirmDeleteMessage() const = 0;

    /**
     * @brief  Gets an appropriate title to use for a menu item editor.
     *
     * @return  A localized editor title string.
     */
    virtual juce::String getEditorTitle() const = 0;

    /**
     * @brief  Menu item data fields that may or may not be editable.
     */
    enum class DataField
    {
        title,
        icon,
        command,
        categories,
        termLaunchOption
    };

    /**
     * @brief  Checks if a data field within this menu item can be edited.
     *
     * @param dataField  The type of data being checked.
     *
     * @return           True if and only if the data field is editable.
     */
    virtual bool isEditable(const DataField dataField) = 0;

    /**
     * @brief  Gets the number of menu items in the folder opened by this menu
     *         item.
     *
     * @return  The number of folder items, or zero if this menu item does not
     *          open a folder.
     */
    virtual int getFolderSize() = 0;

protected:
    /**
     * @brief  Updates the menu item data's saved index value.
     *
     * Use this to update the index value when menu items are moved.
     *
     * @param depth   The level in the menu tree where the index should be
     *                moved.
     *
     * @param offset  The amount to offset the menu item index.
     */
    void updateIndex(const int depth, const int offset);

private:
    MenuIndex index;

    /* Prevents unsafe concurrent access to menu item data. */
    const juce::ReadWriteLock dataLock;
    
    JUCE_DECLARE_NON_COPYABLE(MenuItemData);
};
