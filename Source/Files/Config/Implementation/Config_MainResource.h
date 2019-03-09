#pragma once
/**
 * @file  Config_MainResource.h
 *
 * @brief  Loads general application settings from the config.json file.
 */

#include "Config_FileResource.h"
#include "JuceHeader.h"

namespace Config { class MainResource; }

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
        const final override;
};
