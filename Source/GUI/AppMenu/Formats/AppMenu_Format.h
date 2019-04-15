#pragma once
/**
 * @file  AppMenu_Format.h
 *
 * @brief  Defines the available menu implementations. Each one(besides Invalid)
 *         has a namespace of the same name within the AppMenu namespace where
 *         all of its classes are declared.
 */

namespace AppMenu{ enum class Format; }

enum class AppMenu::Format
{
    // Displays the menu as vertical scrolling menu folder panels:
    Scrolling,
    // Displays the menu as pages of menu item grids:
    Paged,
    // The Format value returned if the menu is somehow null or invalid:
    Invalid
};
