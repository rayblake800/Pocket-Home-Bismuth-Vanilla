#pragma once
#include "ConfigJSON.h"

/**
 * @file MainJSON.h
 * 
 * @brief Loads general application settings from the config.json file.
 */
class MainJSON : public ConfigJSON
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    MainJSON();

    virtual ~MainJSON() { } 
    
    /**
     * @brief   Gets the set of all basic (non-array, non-object) properties 
     *          tracked by this ConfigJSON.
     * 
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<ConfigKey>& getConfigKeys() const override;
};
