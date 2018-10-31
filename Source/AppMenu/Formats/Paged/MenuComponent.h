/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/MenuComponents/MenuComponent.h"

/**
 * @file  MenuFormats/Paged/MenuComponent.h
 *
 * @brief  Creates and arranges application menu folder components for the
 *         paged AppMenu format.
 */
class AppMenu::Paged::MenuComponent : public AppMenu::MenuComponent
{
public:
    MenuComponent() { }

    virtual ~MenuComponent() { }

protected:
    /**
     * @brief  Arranges folders from top to bottom, with the active folder
     *         filling the menu's bounds.
     */
    virtual void updateMenuLayout() final override;

private:
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
};

/* Only include this file directly in the AppMenu implementation! */
#endif
