#pragma once
#include "MainKeys.h"
#include "FileResource.h"

/**
 * @file  MainResource.h
 *
 * @brief  Loads general application settings from the config.json file.
 */
class Config::MainResource : public FileResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    MainResource();

    virtual ~MainResource() { } 
    
private:
    /**
     * @brief   Gets the set of all basic (non-array, non-object) properties 
     *          tracked by this ConfigJSON.
     * 
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() 
        const final override
    {
        return MainKeys::allKeys;
    }
};
