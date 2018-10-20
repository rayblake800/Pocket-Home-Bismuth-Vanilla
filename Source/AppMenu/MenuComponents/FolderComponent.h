/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "MenuComponent.h"
#include "AppMenu.h"

/**
 * @file  FolderComponent.h
 *
 * @brief 
 */
class AppMenu::FolderComponent : public MenuComponent::Folder,
public juce::Button::Listener
{
public:
    FolderComponent(MenuComponent& menuComponent, MenuItem folderItem);
    virtual ~FolderComponent() { }

    virtual MenuItem getFolderItem() const override;
    virtual MenuItem getSelectedItem() const override;
    virtual int getSelectedIndex() const override;

    class ItemButton : public juce::Button
    {
    public:
        ItemButton(MenuItem folderItem);
        ~ItemButton() { }

        virtual MenuItem getMenuItem() const = 0;
    };

protected:

    ItemButton* getButtonComponent(const int index);

private:
    virtual ItemButton* createMenuButton() = 0;
    virtual void updateButtonLayout() = 0;

    juce::OwnedArray<MenuButton> folderButtons;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
