#define APPMENU_IMPLEMENTATION
#include "AppMenu_ItemData.h"

// Gets this menu item's parent folder.
AppMenu::ItemData::Ptr AppMenu::ItemData::getParentFolder() const
{
    return parent;
}


// Gets this menu item's index within its parent folder.
int AppMenu::ItemData::getIndex() const
{
    return index;
}


// Checks if this menu item represents a folder within the menu.
bool AppMenu::ItemData::isFolder() const
{
    return getCommand().isEmpty();
}


// Gets the number of folder items held by this menu item.
int AppMenu::ItemData::getFolderSize() const
{
    return children.size();
}


// Gets a menu item contained in a folder menu item.
AppMenu::ItemData::Ptr AppMenu::ItemData::getChild(const int childIndex) const
{
    if (childIndex < 0 || childIndex >= children.size())
    {
        return nullptr;
    }
    return children[childIndex];
}


// Attempts to insert a new menu item into this folder menu item's array of
// child menu items, saving the change to this folder item's data source.
bool AppMenu::ItemData::insertChild
(const AppMenu::ItemData::Ptr newChild, const int childIndex)
{
    if (childIndex < 0 || childIndex > children.size())
    {
        return false;
    }
    children.insert(childIndex, newChild);
    newChild->parent = this;
    newChild->index = childIndex;
    for (int i = childIndex + 1; i < children.size(); i++)
    {
        children[i]->index++;
    }
    foreachListener([&childIndex](Listener* listener)
    {
        listener->childAdded(childIndex);
    });
    return true;
}


// Removes this menu item from its folder, optionally deleting it from its data
// source.
bool AppMenu::ItemData::remove(const bool updateSource)
{
    if (parent == nullptr || index < 0)
    {
        return false;
    }
    parent->children.remove(index);
    for (int i = index; i < parent->children.size(); i++)
    {
        parent->children[i]->index--;
    }
    parent->foreachListener([this](Listener* listener)
    {
        listener->childRemoved(index);
    });
    parent = nullptr;
    index = -1;
    if (updateSource)
    {
        deleteFromSource();
    }
    return true;
}


// Swaps the positions of two menu items, saving the change to this menu item's
// data source if both items may be moved by the user.
bool AppMenu::ItemData::swapChildren(const int childIdx1, const int childIdx2)
{
    const int maxIndex = getFolderSize() - 1;
    const int maxMovable = getMovableChildCount() - 1;
    if ((childIdx1 < 0 && childIdx2 < 0)
            || (childIdx1 > maxIndex || childIdx2 > maxIndex))
    {
        return false;
    }
    children.swap(childIdx1, childIdx2);
    children[childIdx1]->index = childIdx1;
    children[childIdx2]->index = childIdx2;
    foreachListener([&childIdx1, &childIdx2](Listener* listener)
    {
        listener->childrenSwapped(childIdx1, childIdx2);
    });
    if (childIdx1 <= maxMovable && childIdx2 <= maxMovable)
    {
        saveChanges();
    }
    return true;
}


// Removes all references to this listener from the menu items it tracks.
AppMenu::ItemData::Listener::~Listener()
{
    if (trackedItemData != nullptr)
    {
        trackedItemData->removeListener(this);
    }
}


// Connects a new Listener object to this ItemData.
void AppMenu::ItemData::addListener(Listener* newListener)
{
    if (newListener != nullptr)
    {
        listeners.add(newListener);
        newListener->trackedItemData = this;
    }
    else
    {
        DBG("AppMenu::ItemData::" << __func__ << ": Listener was null!");
        jassertfalse;
    }
}


// Disconnects a Listener object from this ItemData.
void AppMenu::ItemData::removeListener(Listener* toRemove)
{
    if (toRemove != nullptr)
    {
        if (toRemove->trackedItemData == this)
        {
            toRemove->trackedItemData = nullptr;
            listeners.removeAllInstancesOf(toRemove);
        }
        else
        {
            DBG("AppMenu::ItemData::" << __func__ <<
                    ": Listener was not listening to this item data!");
            jassertfalse;
        }
    }
    else
    {
        DBG("AppMenu::ItemData::" << __func__ << ": Listener was null!");
        jassertfalse;
    }
}


// Signal to all listeners tracking this ItemData that the item has changed.
void AppMenu::ItemData::signalDataChanged(const DataField changedField)
{
    foreachListener([changedField](Listener* listener)
    {
        listener->dataChanged(changedField);
    });
}


// Runs an arbitrary operation on each listener tracking this ItemData.
void AppMenu::ItemData::foreachListener
(const std::function<void(Listener*)> listenerAction)
{
    for (Listener* listener : listeners)
    {
        listenerAction(listener);
    }
}
