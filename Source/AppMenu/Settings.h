#pragma once
#include "AppMenu/AppMenu.h"


/**
 * @file  AppMenu/Settings.h
 *
 * @brief  Gets and sets user-configurable menu settings such as selected menu
 *         format and menu dimensions.
 */
namespace AppMenu
{
    namespace Settings
    {

        static AppMenu::Format getMenuFormat();

        static void setMenuFormat(const AppMenu::Format newFormat);
    }

};
