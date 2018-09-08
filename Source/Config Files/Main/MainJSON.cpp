#include "MainJSON.h"
#include "MainConfigKeys.h"

/* SharedResource object key */
static const juce::Identifier resourceKey;

/* Filename of the JSON configuration file */
static const constexpr char * configFilename = "config.json";

MainJSON::MainJSON() : ConfigJSON(resourceKey, configFilename) { } 

/*
 * Gets the set of all basic (non-array, non-object) properties tracked by
 * this ConfigJSON.
 */
const std::vector<ConfigKey>& MainJSON::getConfigKeys() const
{
    return MainConfigKeys::allKeys();
}

