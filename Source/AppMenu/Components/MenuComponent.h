/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/Components/FolderComponent.h"
#include "AppMenu/Data/MenuItem.h"
#include "AppMenu/Components/Editors/PopupEditor.h"
#include "AppMenu/AppMenu.h"
#include "JuceHeader.h"

/**
 * @file AppMenu/Components/MenuComponent.h
 *
 * @brief  Creates and arranges application menu folder components.
 *
 * The MenuComponent provides the interface used by AppMenu::Controller
 * and AppMenu::InputHandler to interact with menu components. It opens,
 * closes, and positions child FolderComponent objects when folder MenuItem
 * objects are opened, closed, or otherwise adjusted. It allows other objects
 * to access open folder components within the menu. It updates the positions of
 * its FolderComponents as necessary, optionally animating component
 * transitions.  Finally, when PopupEditor components are created, it holds
 * those as well, adding them as child components, and keeping them centered
 * within its bounds.
 *
 * MenuComponent is an abstract class.  Each AppMenu format should have its own
 * MenuComponent implementation that defines exactly where child folders should
 * be placed when the component layout is updated.
 */
class AppMenu::MenuComponent : public juce::Component
{
public:
    MenuComponent() { }

    virtual ~MenuComponent() { }

    /**
     * @brief  Gets the MenuItem for the current active folder component.
     *
     * @return  An object holding the last opened menu folder's data.
     */
    MenuItem getActiveFolder() const;

    /**
     * @brief  Creates, shows, and activates a new folder component.
     *
     * @param folderItem  The new folder's menu data.
     */
    void openFolder(MenuItem folderItem);

    /**
     * @brief  Gets the number of open folder components.
     *
     * @return   The open folder count.
     */
    int openFolderCount() const;

    /**
     * @brief  Closes the current active folder as long as more than one
     *         folder is open. 
     */
    void closeActiveFolder();

    /**
     * @brief  Updates the positions and sizes of all open folder components.
     *
     * @param animateTransition  Whether the change in folder bounds should be
     *                           animated.
     */
    void updateMenuLayout(const bool animate = true);

    /**
     * @brief  Updates the menu's bounds when its parent component's bounds
     *         change.
     *
     * @param parentBounds  The new bounds of the menu's parent component.
     */
    virtual void parentResized(const juce::Rectangle<int> parentBounds) = 0;

    /**
     * @brief  Gets an open folder component from the list of open folders.
     *
     * @param folderIndex  The index of a folder component in the list of open
     *                     folders.
     *
     * @return             The component's address, or nullptr if the index is
     *                     out of bounds.
     */
    FolderComponent* getOpenFolder(const int folderIndex) const;

    /**
     * @brief  Saves a new popup editor component, and adds it to the main menu
     *         component.
     *
     * @param newEditor  The new menu editor component to save and show.
     */
    void saveAndShowEditor(PopupEditor* newEditor);

    /**
     * @brief  Checks if the menu is currently showing a menu editor component.
     *
     * @return  Whether a popup menu editor box is currently showing.
     */
    bool showingEditor() const;

    /**
     * @brief  If a menu editor component exists, this will remove it from its
     *         parent component.
     */
    void removeEditor();

private:
    /**
     * @brief  Creates a new folder component.
     *
     * @param folderItem  The folder item that will define the new folder
     *                    component.
     *
     * @return            The new component, as some concrete subclass of
     *                    AppMenu::FolderComponent.
     */
    virtual FolderComponent* createFolderComponent(MenuItem folderItem) 
        const = 0;
    
    /**
     * @brief  Finds the initial bounds to apply to a newly created folder
     *         component.
     *
     * @param newFolderIndex  The index the newly opened folder component will
     *                        have in the list of open folders.
     *
     * @return                The initial bounds to apply to the component 
     *                        before fully updating the folder layout.
     */
    virtual juce::Rectangle<int> initialFolderBounds(const int newFolderIndex)
        const = 0;

    /**
     * @brief  Prepares to update the folder layout, updating cached data as
     *         necessary.  
     *
     * This does nothing by default, but MenuComponent subclasses may override 
     * it to define actions that should be taken at the start of each call to
     * updateMenuLayout.
     *
     * @param closingFolder  Whether the active folder is being closed.
     */
    virtual void layoutUpdateStarting(const bool closingFolder = false) { }

    /**
     * @brief   Finds the bounds where a menu folder should be placed.
     *
     * @param folderIndex    The index of an open folder component.
     *
     * @param closingFolder  Whether the active folder (not necessarily this
     *                       folder!) is about to be closed.
     *
     * @return               The bounds within the MenuComponent where the
     *                       folder should be placed.
     */
    virtual juce::Rectangle<int> getFolderBounds(const int folderIndex,
            const bool closingFolder = false) const = 0;

    /**
     * @brief  Gets the duration in milliseconds to animate folder transitions.
     *
     * @return   The AppMenu format's animation duration.
     */
    virtual int getAnimationDuration() const = 0;

    /**
     * @brief  Updates the positions and sizes of all open folder components.
     *
     * @param animateTransition  Whether the change in folder bounds should be
     *                           animated.
     *
     * @param closingFolder      Whether the last folder component is being
     *                           closed.
     */
    void updateMenuLayout(const bool animate, const bool closingFolder);

    /**
     * @brief  Updates the menu layout when the component is resized.
     */
    virtual void resized() final override;
    
    /* The list of open folder components */
    juce::OwnedArray<FolderComponent> openFolders;

    /* Holds any popup menu editor component: */
    std::unique_ptr<PopupEditor> menuEditor = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
};

/* Only include this file directly in the AppMenu implementation! */
#endif
