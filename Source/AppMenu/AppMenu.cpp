#include "AppMenu.h"
#include "Controller/MainComponent.h"
#include "MenuFormats/Paged/Controller.h"
#include "MenuFormats/Scrolling/Controller.h"
#include "JuceHeader.h"

/*
 * Creates an AppMenu::MainComponent with a specific initial format.
 */
AppMenu::MainComponent* AppMenu::createAppMenu(const Format menuFormat)
{
    switch(menuFormat)
    {
        case Format::Paged:
            return new MainComponent(new Paged::Controller());
        case Format::Scrolling:
            return new MainComponent(new Scrolling::Controller());
        default:
            DBG("AppMenu::" << __func__ << ": Invalid menu format!");
            jassertfalse;
            return nullptr;
    }
}

/*
 * Changes the format of an existing AppMenu::MainComponent.
 */
void AppMenu::changeMenuFormat
(const MainComponent* appMenu, const Format newFormat)
{
    if(appMenu == nullptr)
    {
        DBG("AppMenu::" << __func__ << ": Existing menu was null!");
        jassertfalse;
    }
    else if(newFormat == appMenu->getMenuFormat())
    {
        DBG("AppMenu::" << __func__ 
                << ": New format matches existing format,  taking no action.");
    }
    else
    {
         switch(newFormat)
        {
            case Format::Paged:
                appMenu->initMenu(new Paged::Controller());
            case Format::Scrolling:
                appMenu->initMenu(new Scrolling::Controller());
            default:
                DBG("AppMenu::" << __func__ << ": Invalid menu format!");
                jassertfalse;
        }
    }
}
