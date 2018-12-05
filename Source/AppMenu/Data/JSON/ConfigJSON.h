#ifndef APPMENU_IMPLEMENTATION
    #error __file__ included outside of AppMenu implementation.
#endif

#pragma once
#include "Config/FileResource.h"
#include "Config/FileHandler.h"
#include "AppMenu/AppMenu.h"

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
     * @return  The list of keys defined in AppMenu/Data/JSON/ConfigKeys.h
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() const final 
        override;
};
