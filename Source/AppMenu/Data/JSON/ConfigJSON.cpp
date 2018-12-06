#define APPMENU_IMPLEMENTATION
#include "AppMenu/Data/JSON/ConfigKeys.h"
#include "AppMenu/Data/JSON/ConfigJSON.h"

/* SharedResource object key */
const juce::Identifier AppMenu::ConfigJSON::resourceKey 
        = "AppMenu::ConfigJSON";

/* JSON configuration file name */
static const constexpr char * jsonFilename = "menuOptions.json";

/*
 * Initializes the menu data tree.
 */
AppMenu::ConfigJSON::ConfigJSON() :
Config::FileResource(resourceKey, jsonFilename)
{
    loadJSONData(); 
}

/*
 * Gets all parameters with basic data types tracked by this ConfigFile.
 */
const std::vector<Config::DataKey>& AppMenu::ConfigJSON::getConfigKeys() const 
{
    return ConfigKeys::allKeys;
}



