#include "MainResource.h"

/* Filename of the JSON configuration file */
static const constexpr char * configFilename = "config.json";

/* SharedResource object key */
const juce::Identifier Config::MainResource::resourceKey 
    = "Config::MainResource";

Config::MainResource::MainResource() : FileResource(resourceKey, configFilename)
{
    loadJSONData();
}
