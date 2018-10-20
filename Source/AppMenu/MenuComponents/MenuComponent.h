/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "Controller.h"
#include "AppMenu.h"

/**
 * @file MenuComponent.h
 *
 * @brief 
 */
class AppMenu::MenuComponent : public Controller::ControlledMenu
{
public:
    MenuComponent(Controller& controller);

    virtual ~MenuComponent() { }

    virtual MenuItem getSelectedItem() override;

    virtual MenuItem getActiveFolder() override;

    class Folder : public juce::Component
    {
    public:
        Folder(MenuComponent& menuComponent, MenuItem folderItem);

        virtual ~Folder() { }

        virtual MenuItem getFolderItem() const = 0;

        virtual MenuItem getSelectedItem() const = 0;

        virtual int getSelectedIndex() const = 0; 
        
    protected:
        void signalItemClicked(MenuItem clickedItem, const bool rightClicked);

        void signalFolderClicked
        (const bool rightClicked, const int closestIndex = -1);

        virtual void updateFolderLayout() = 0;

    private:
        virtual void checkFolderStructure() override;

        virtual void addMenuItemComponent(MenuItem toAdd) override;

        virtual void removeMenuItemComponent(MenuItem toRemove) override;

        virtual void swapMenuItemComponents
        (MenuItem swapItem1, MenuItem swapItem2) override;
    };
protected:
    virtual Folder* createFolderComponent(MenuItem folderItem) = 0;

    virtual Folder* findFolderComponent(MenuItem folderItem) = 0;

    virtual void updateMenuLayout() = 0;

    int openFolderCount() const;

    Folder* getOpenFolder(const int folderIndex) const;

private:
    virtual void openFolder(MenuItem folderItem) override;

    virtual void closeActiveFolder() override;

    virtual void checkFolderStructure() override;

    virtual void addMenuItemComponent(MenuItem toAdd) override;

    virtual void removeMenuItemComponent(MenuItem toRemove) override;

    virtual void swapMenuItemComponents(MenuItem swapItem1, MenuItem swapItem2)
        override;

    virtual void mouseDown(const juce::MouseEvent& event) override;

    virtual void buttonClicked(juce::Button* button) override;

    virtual bool keyPressed(const juce::KeyPress& key) override; 

    virtual void resized() override;
    
    juce::OwnedArray openFolders;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
