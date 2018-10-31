/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/MenuComponents/MenuComponent.h"

/**
 * @file  MenuFormats/Tabbed/MenuComponent.h
 *
 * @brief  Creates and arranges application menu folder components for the
 *         tabbed AppMenu format.
 */
class AppMenu::Tabbed::MenuComponent : public AppMenu::MenuComponent
{
public:
    MenuComponent() { }

    virtual ~MenuComponent() { }

protected:
    /**
     * @brief  Arranges folders from left to right, with the selected item of
     *         the active folder centered.
     */
    virtual void updateMenuLayout() final override;

private:
    /**
     * @brief  Creates a new Tabbed::FolderComponent for an opened folder
     *         menu item.
     *
     * @param folderItem  The folder component's menu data source.
     *
     * @return  The new Tabbed::FolderComponent object.
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
};

/* Only include this file directly in the AppMenu implementation! */
#endif
