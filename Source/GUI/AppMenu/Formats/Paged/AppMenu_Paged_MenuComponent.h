#ifndef APPMENU_IMPLEMENTATION
  #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_Paged_MenuComponent.h
 *
 * @brief  Creates and arranges application menu folder components for the
 *         paged AppMenu format.
 */

#include "AppMenu_MenuComponent.h"
#include "AppMenu_ConfigFile.h"
#include "Layout_Component_Manager.h"
#include "Widgets_NavButton.h"

namespace AppMenu { namespace Paged { class MenuComponent; } }

/**
 *  In the Paged::MenuComponent, only the active page of the last opened 
 * FolderComponent is visible. The MenuComponent is responsible for ensuring
 * each folder page is the same size as the MenuComponent, and the correct page
 * is currently showing.  The MenuComponent also listens for changes to the
 * paged menu grid dimensions, re-positioning its folder components if the
 * grid dimensions change.
 * 
 *  The Paged::MenuComponent also creates, shows, and holds the navigation
 * buttons used to change the selected folder page.
 */
class AppMenu::Paged::MenuComponent : public AppMenu::MenuComponent,
    public ConfigFile::Listener
{
public:
    /**
     * @brief  Creates the menu component and initializes the navigation 
     *         buttons.
     */
    MenuComponent();
    
    virtual ~MenuComponent() { }

    /**
     * @brief  Adds a listener object to the menu's navigation buttons. 
     *
     * @param listener  The Listener that will handle button clicks on the 
     *                  navigation buttons.
     */
    void addNavButtonListener(juce::Button::Listener* listener);

private:
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
        const final override;

    /**
     * @brief  Prepares to update the folder layout, showing or hiding the
     *         navigation buttons based on the needs of the new layout.
     *
     * @param closingFolder  Whether the active folder is being closed.
     */
    virtual void layoutUpdateStarting(const bool closingFolder = false)
        final override;

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
            const bool closingFolder = false) const final override;

    /**
     * @brief  Gets the duration in milliseconds to animate folder transitions.
     *
     * @return   The AppMenu format's animation duration.
     */
    virtual int getAnimationDuration() const final override;

    /**
     * @brief  Creates a new Paged::FolderComponent for an opened folder
     *         menu item.
     *
     * @param folderItem  The folder component's menu data source.
     *
     * @return  The new Paged::FolderComponent object.
     */
    virtual AppMenu::FolderComponent* createFolderComponent(MenuItem folderItem)
        const final override;

    /**
     * @brief  Updates the menu's bounds whenever its parent component is
     *         resized.
     *
     * @param parentBounds  The new bounds of the parent component.
     */
    virtual void parentResized(const juce::Rectangle<int> parentBounds)
        final override;

    /**
     * @brief  Updates the menu layout when the paged menu's grid dimensions 
     *         change.
     *
     * @param propertyKey  The paged menu's row or column count key.
     */
    virtual void configValueChanged(const juce::Identifier& propertyKey)
        final override;

    /* Sets the menu's bounds relative to the application window. */
    Layout::Component::Manager boundsManager;

    /* Navigation button that closes the active folder: */
    Widgets::NavButton upButton;
    /* Navigation button that decrements the selected folder page: */
    Widgets::NavButton leftButton;
    /* Navigation button the increments the selected folder page: */
    Widgets::NavButton rightButton;
};
