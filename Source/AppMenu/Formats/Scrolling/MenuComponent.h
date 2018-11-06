/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "ConfigurableComponent.h"
#include "AppMenu/Data/ConfigFile.h"
#include "AppMenu/Components/MenuComponent.h"

/**
 * @file  MenuFormats/Scrolling/MenuComponent.h
 *
 * @brief  Creates and arranges application menu folder components for the
 *         Scrolling AppMenu format.
 *
 * The Scrolling::MenuComponent arranges open FolderComponent objects from left
 * to right. It attempts to keep the selected menu item in the active folder
 * as centered as possible, only moving off-center when doing so would keep more
 * of the menu visible.  It ensures each FolderComponent is wide enough to
 * fully print the titles of all MenuButtons, and tall enough to fit the number
 * of rows specified by the AppMenu config file.
 */
class AppMenu::Scrolling::MenuComponent : public AppMenu::MenuComponent,
    public ConfigurableComponent, public ConfigFile::Listener
{
public:
    /**
     * @brief  Creates the menu component, listening to the AppMenu::ConfigFile
     *         for changes to the number of rows to fit on screen.
     */
    MenuComponent();

    virtual ~MenuComponent() { }

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
     * @brief  Prepares to update the folder layout, updating cached layout 
     *         values.
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
     * @brief  Creates a new Scrolling::FolderComponent for an opened folder
     *         menu item.
     *
     * @param folderItem  The folder component's menu data source.
     *
     * @return            The new Scrolling::FolderComponent object.
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
     * @brief  Updates the menu layout when the scrolling menu row count
     *         changes.
     *
     * @param propertyKey  The scrolling menu row count key.
     */
    virtual void configValueChanged(const juce::Identifier& propertyKey)
        final override;

    /* Cached folder widths to reuse when calculating layout changes: */
    juce::Array<int> folderWidths;

    /* Holds the position of the last folder's right edge, relative to the
       MenuComponent's left edge: */
    int folderRightEdge;

    /* Holds the number of visible button rows to fit on the screen: */
    int maxRows;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
