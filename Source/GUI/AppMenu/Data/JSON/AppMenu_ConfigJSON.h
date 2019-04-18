#ifndef APPMENU_IMPLEMENTATION
    #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_ConfigJSON.h
 *
 * @brief  Reads and writes application menu configuration values.
 */

#include "Config_FileResource.h"
#include "Config_FileHandler.h"
#include "AppMenu.h"

namespace AppMenu { class ConfigJSON; }

/**
 * @brief  Reads from and writes to the JSON file that sets the AppMenu's
 *         format, the number of items the menu shows on screen at once, and the
 *         application launch timeout period.
 *
 *  ConfigJSON is a SharedResource class, used to create the single object
 * instance that accesses the menuOptions.json file. ConfigJSON should only be
 * used directly by AppMenu::ConfigFile objects.
 */
class AppMenu::ConfigJSON : public Config::FileResource
{
public:
    // SharedResource object key
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes menu configuration data.
     */
    ConfigJSON();

    virtual ~ConfigJSON() { }

private:
    /**
     * @brief  Gets all parameters with basic data types tracked by this
     *         ConfigFile.
     *
     * @return  The list of keys defined in AppMenu_ConfigKeys.h
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() const final
        override;
};
