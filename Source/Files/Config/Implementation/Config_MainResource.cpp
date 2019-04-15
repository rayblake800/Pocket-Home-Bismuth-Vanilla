#include "Config_MainResource.h"
#include "Config_MainKeys.h"

// Filename of the JSON configuration file
static const constexpr char * configFilename = "config.json";

// SharedResource object key
const juce::Identifier Config::MainResource::resourceKey
    = "Config::MainResource";


Config::MainResource::MainResource() :
FileResource(resourceKey, configFilename)
{
    loadJSONData();
}


// Gets the set of all basic(non-array, non-object) properties tracked by this
// ConfigJSON.
const std::vector<Config::DataKey>& Config::MainResource::getConfigKeys() const
{
    return MainKeys::allKeys;
}
