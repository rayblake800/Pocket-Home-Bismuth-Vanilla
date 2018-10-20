/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "JuceHeader.h"
#include "AppMenu.h"

/**
 * @file  ItemData.h
 *
 * @brief  Reads and writes properties of a menu item in the application menu. 
 *
 * All menu items have a title string and an icon name or path.  Menu items
 * define either an application to launch or a folder of other menu items.
 * 
 * Menu item data is stored through shared, referenced-counted objects.  Like
 * other UI elements, menu item data is absolutely not threadsafe, and should
 * not be handled outside of the Juce message thread.
 * 
 */
class AppMenu::ItemData : public juce::ReferenceCountedObject
{
public:
    
    /* Custom reference-counting pointer object type. */
    typedef juce::ReferenceCountedObjectPtr<ItemData> Ptr;

    ItemData() { }
    
    virtual ~ItemData() { }

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
     * @brief  Gets an optional unique ID tied to this menu item.
     *
     * @return   A unique ID string, or an empty string if the menu item has no
     *           ID.
     */
    virtual juce::String getID() const { return juce::String(); }

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
     *                    activated.
     */
    virtual void setCommand(const juce::String& newCommand) = 0;

    /**
     * @brief  Sets if this menu item runs its command in a new terminal window.
     *
     * @param launchInTerm  True to run any launch command assigned to this
     *                      menu item within a new terminal window, false to run
     *                      menu commands normally.
     */
    virtual void setLaunchedInTerm(const bool launchInTerm) = 0;

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
    ItemData::Ptr getParentFolder() const;

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
    virtual int getMovableChildCount() const = 0;

    /**
     * @brief  Checks if this menu item could be moved within its folder,
     *         assuming that another movable menu item exists that could be
     *         swapped with this one.
     *
     * @return  Whether this menu item could be moved.
     */
    virtual bool isMovable() const = 0;

    /**
     * @brief  Gets a menu item contained in a folder menu item.
     *
     * @param childIndex  The index of the child menu item to get.
     *
     * @return            The child menu item, or nullptr if the index is out of
     *                    bounds or this menu item is not a folder.
     */
    ItemData::Ptr getChild(const int childIndex) const;

    /**
     * @brief  Gets all menu items contained in a folder menu item.
     *
     * @return  All menu items within the folder, or an empty array if this
     *          menu item is not a folder.
     */
    juce::Array<ItemData::Ptr> getChildren() const;

    /**
     * @brief  Attempts to insert a new menu item into this folder menu item's
     *         array of child menu items, saving the change to this folder
     *         item's data source.
     *
     * @param newChild    The new child menu item to insert.
     *
     * @param childIndex  The index in the folder where the menu item should
     *                    be inserted.  This should be between 0 and
     *                    getMovableChildCount(), inclusive.
     *
     * @return            True if the new menu item was inserted, false if the
     *                    index was out of bounds or this menu item is not a
     *                    folder, and the new item could not be inserted.
     */
    bool insertChild(const ItemData::Ptr newChild, const int childIndex);

    /**
     * @brief  Removes this menu item from its folder, deleting it from its
     *         data source.
     *
     * @return       True if a menu item was removed, false if this menu item
     *               was not located in a folder.
     */
    bool remove();

    /**
     * @brief  Swaps the positions of two menu items, saving the change to this
     *         menu item's data source.
     *
     * @param childIdx1  The index of the first child item to move.
     *
     * @param childIdx2  The index of the second child item to move.
     *
     * @return  True if the child menu items were swapped, false if the indices
     *          did not specify two valid menu items within the folder's movable
     *          child menu items.
     */
    bool swapChildren(const int childIdx1, const int childIdx2);
    
    /**
     * @brief  Saves all changes to this menu item back to its data source.
     */
    virtual void saveChanges() = 0;
    
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
    virtual bool isEditable(const DataField dataField) const = 0;

    /**
     * @brief  Receives updates when tracked menu items change. 
     *
     * Listeners connected to a menu item through its addListener method
     * are notified whenever the menu item's data changes, whenever the menu
     * item is removed from the menu, and whenever the menu item's child 
     * folder items are added, removed, or swapped.
     *
     * Listeners automatically disconnect from their tracked ItemData sources
     * when they are destroyed.  ItemData is also automatically disconnected
     * from all listeners when it is removed from the menu, after calling
     * dataRemoved on each listener.
     */
    class Listener
    {
    public:
        /* Allows ItemData objects to add themselves to this Listener's tracked
           item list when addListener is called. */
        friend class ItemData;

        Listener() { }

        /**
         * @brief  Removes all references to this listener from the menu items
         *         it tracks.
         */
        virtual ~Listener();

    private:
        /**
         * @brief  This method will be called once any time a new child folder
         *         item is added to a tracked ItemData object.
         *
         * @param folderItem  The tracked folder item.
         *
         * @param childIndex  The index where the new child item was inserted.
         */
        virtual void childAdded(ItemData::Ptr folderItem, 
                const int childIndex) { }

        /**
         * @brief  This method will be called once any time a child folder item
         *         is removed from a tracked ItemData object.
         *
         * @param folderItem    The tracked folder item.
         *
         * @param removedIndex  The former index of the tracked item's removed
         *                      child item.
         */
        virtual void childRemoved(ItemData::Ptr folderItem,
                const int removedIndex) { }

        /**
         * @brief  This method will be called once any time two child folder
         *         items of a tracked ItemData object are swapped.
         *
         * @param folderItem  The tracked folder item.
         *
         * @param swapIndex1  The index of the first swapped child item.
         *
         * @param swapIndex2  The index of the second swapped child item.
         */
        virtual void childrenSwapped(ItemData::Ptr folderItem,
                const int swapIndex1, const int swapIndex2) { }

        /**
         * @brief  This method will be called once any time a tracked ItemData
         *         object is about to be removed from the menu.
         *
         * @param removedItem   The tracked object that will be removed.
         */
        virtual void removedFromMenu(ItemData::Ptr removedItem) { }

        /**
         * @brief  This method will be called once whenever any other element of
         *         a tracked ItemData object's data changes.
         *
         * @param changedItem  The tracked ItemData that has changed in some
         *                     way.
         */
        virtual void dataChanged(ItemData::Ptr changedItem) { }

        /* All ItemData objects tracked by this Listener. */
        juce::Array<ItemData::Ptr> trackedItemData;
    };

    /**
     * @brief  Connects a new Listener object to this ItemData.
     *
     * @param newListener  A listener that will track this ItemData.
     */
    void addListener(Listener* newListener);

    /**
     * @brief  Disconnects a Listener object from this ItemData.
     *
     * @param toRemove  A listener that will no longer track this ItemData.
     */
    void removeListener(Listener* toRemove);

protected:
    /**
     * @brief  Signal to all listeners tracking this ItemData that the item
     *         has changed.
     */
    void signalDataChanged();

private:
    /**
     * @brief  Runs an arbitrary operation on each listener tracking this
     *         ItemData.
     *
     * @param listenerAction  An action to perform on all listeners. Each time 
     *                        it is called, a different listener will be passed
     *                        to it as its sole parameter.
     */
    void foreachListener(const std::function<void(Listener*)> listenerAction);

    /**
     * @brief  Deletes this menu item data from its data source.
     */
    virtual void deleteFromSource() = 0;

    /* The folder menu item that contains this menu item. */
    ItemData::Ptr parent = nullptr;

    /* The menu item's index within its parent folder. */
    int index = -1;

    /* Menu items contained in this menu item, if it is a folder. */
    juce::Array<ItemData::Ptr> children;

    /* Objects that should be notified if this menu item changes. */
    juce::Array<Listener*> listeners;

    JUCE_DECLARE_NON_COPYABLE(ItemData);
};

/* Only include this file directly in the AppMenu implementation! */
#endif
