#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file AppMenu_ConfigJSON.h
 *
 * @brief  Reads from and writes to the JSON file that sets the AppMenu's format
 *         and the number of items the menu shows on screen at once.
 */

#include "Config/FileResource.h"
#include "Config/FileHandler.h"
#include "AppMenu.h"

namespace AppMenu { class ConfigJSON; }

/**
 *  ConfigJSON is a SharedResource class, used to create the single object
 * instance that accesses the menuOptions.json file. ConfigJSON should only be 
 * used directly by AppMenu::ConfigFile objects.
 */
class AppMenu::ConfigJSON : public Config::FileResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes menu configuration data.
     */
    ConfigJSON();

    virtual ~ConfigJSON() { }

private:
    /**
     * @brief   Gets all parameters with basic data types tracked by this
     *          ConfigFile.
     * 
     * @return  The list of keys defined in AppMenu_ConfigKeys.h
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() const final 
        override;
};
