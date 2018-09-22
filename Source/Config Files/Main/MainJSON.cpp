#include "MainJSON.h"
#include "MainConfigKeys.h"

/* SharedResource object key */
const juce::Identifier MainJSON::resourceKey = "MainJSON";

/* Filename of the JSON configuration file */
static const constexpr char * configFilename = "config.json";

MainJSON::MainJSON() : ConfigJSON(resourceKey, configFilename) 
{
    loadJSONData();
} 

/*
 * Gets the set of all basic (non-array, non-object) properties tracked by
 * this ConfigJSON.
 */
const std::vector<ConfigKey>& MainJSON::getConfigKeys() const
{
    return MainConfigKeys::allKeys;
}

