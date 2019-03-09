#pragma once
/**
 * @file  Theme_Colour_UICategory.h
 *
 * @brief  Defines broad categories of UI element types used to assign default
 *         color values.
 */

namespace Theme { namespace Colour { enum class UICategory; } }

/**
 * @brief  All Juce ColourId values used by this program are assigned to one of 
 *         these categories.
 */
enum class Theme::Colour::UICategory
{
    windowBackground = 0,
    widgetBackground,
    widgetOff,
    widgetOn,
    menuBackground,
    outline,
    focusedOutline,
    textField,
    text,
    highlightedTextField,
    highlightedText,
    none
};
