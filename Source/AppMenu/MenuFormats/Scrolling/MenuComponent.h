/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/MenuComponents/MenuComponent.h"

class AppMenu::Scrolling::MenuComponent : public AppMenu::MenuComponent
{
public:
    MenuComponent() { }
    virtual ~MenuComponent() { }

protected:
    virtual void updateMenuLayout() final override;

private:
    virtual AppMenu::FolderComponent* createFolderComponent(MenuItem folderItem)
        const final override;

    virtual void parentResized(const juce::Rectangle<int> parentBounds)
        final override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
