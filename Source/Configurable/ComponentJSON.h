#pragma once
#include "Config/FileResource.h"
#include "Config/DataKey.h"
#include "ComponentSettings.h"

/**
 * @file ComponentJSON
 *
 * @brief Reads and writes component settings in the components.json file.
 */
class ComponentJSON : public Config::FileResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    ComponentJSON();

    virtual ~ComponentJSON() { }

    /**
     * @brief   Gets the configured settings for a particular component.
     * 
     * @param componentKey  A configurable UI component's key string.
     * 
     * @return  The properties defined for that component.
     */
    ComponentSettings getComponentSettings
    (const juce::Identifier& componentKey) const;

private:
    /**
     * @brief  Copies all ComponentSettings data back to the JSON file.
     */
    virtual void writeDataToJSON() final override;

    /**
     * @brief   Gets the key string and data type for each basic value stored in
     *          components.json.
     * 
     * @return  Key strings and data types for each bool, int, String, or
     *          double stored in components.json.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() 
        const final override;

    /* Stores all component settings loaded from the component config file */
    std::map<juce::Identifier, ComponentSettings> components;
};

