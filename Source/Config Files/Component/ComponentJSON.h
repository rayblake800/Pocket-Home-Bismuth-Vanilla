#pragma once
#include "ConfigKey.h"

/**
 * @file ComponentJSON
 *
 * @brief
 */
class ComponentJSON : public ConfigFile
{
public:
    ComponentJSON();

    virtual ~ComponentJSON() { }

    /**
     * Gets the configured settings for a particular component.
     * 
     * @param componentKey  A configurable UI component's key string.
     * 
     * @return              the properties defined for that component.
     */
    ComponentSettings getComponentSettings(juce::String componentKey);

private:
    /**
     * Copy all ComponentSettings data back to the JSON file.
     */
    virtual void writeDataToJSON() final override;

    /**
     * Gets the string key and data type for each basic value stored in
     * components.json.
     * 
     * @return  Key strings and data types for each bool, int, String, or
     *          double stored in components.json.
     */
    virtual const std::vector<DataKey>& getDataKeys() const final override;

    //Stores all component settings loaded from the component config file
    std::map<juce::String, ComponentSettings> components;
};

