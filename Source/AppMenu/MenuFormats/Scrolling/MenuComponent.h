/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "ConfigurableComponent.h"
#include "AppMenu/MenuComponents/MenuComponent.h"

/**
 * @file  MenuFormats/Scrolling/MenuComponent.h
 *
 * @brief  Creates and arranges application menu folder components for the
 *         scrolling AppMenu format.
 */
class AppMenu::Scrolling::MenuComponent : public AppMenu::MenuComponent,
    public ConfigurableComponent
{
public:
    MenuComponent();

    virtual ~MenuComponent() { }

    /**
     * @brief  Arranges folders from left to right, with the selected item of
     *         the active folder centered.
     *
     * @param animateTransition  Whether changes to the menu layout should be
     *                           animated.
     */
    void updateMenuLayout(bool animateTransition);

private:
    /**
     * @brief  Arranges folders from left to right, with the selected item of
     *         the active folder centered.  This will always animate any
     *         changes to the menu layout.
     */
    virtual void updateMenuLayout() final override;

    /**
     * @brief  Creates a new Scrolling::FolderComponent for an opened folder
     *         menu item.
     *
     * @param folderItem  The folder component's menu data source.
     *
     * @return  The new Scrolling::FolderComponent object.
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
