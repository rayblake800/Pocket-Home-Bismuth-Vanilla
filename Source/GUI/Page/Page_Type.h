#pragma once
/**
 * @file  Page_Type.h
 *
 * @brief  Defines all types of Page::Component that other pages may create.
 */

namespace Page { enum class Type; }

/**
 * @brief  Defines each subclass of Page::Component that another page may
 *         create.
 */
enum class Page::Type
{
    power,
    fel,
    quickSettings,
    settingsList,
    inputSettings,
    setPassword,
    removePassword,
    colourSettings,
    homeSettings,
#ifdef WIFI_SUPPORTED
    wifiConnection
#endif
};
