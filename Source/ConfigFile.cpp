/*
  ==============================================================================

    ConfigFile.cpp
    Created: 26 Dec 2017 10:31:17am
    Author:  anthony

  ==============================================================================
 */

#include "ConfigFile.h"
/*
  ==============================================================================

    ConfigFile.h
    Created: 26 Dec 2017 10:31:17am
    Author:  anthony

  ==============================================================================
 */
#include "../JuceLibraryCode/JuceHeader.h"
#include "Utils.h"
const std::map<ConfigFile::ComponentType,String> ConfigFile::componentKeys=
        ConfigFile::setComponentKeys(); 

ConfigFile::~ConfigFile() {
}

ConfigFile* ConfigFile::getInstance() {
    static ConfigFile instance;
    return &instance;
}

void ConfigFile::writeChanges() {

}

ConfigFile::ConfigFile() {
    String path = getHomePath() + String(CONFIG_PATH);
    if (!fileExists(path)) {
        loadDefaultConfig();
        return;
    }
    File configFile = File(path);
    var jsonConfig = JSON::parse(configFile);
    if (jsonConfig.getProperty("pages", var::null) != var::null) {
        //found a Marshmallow Pocket-Home config file, update it.
        updateLegacyConfig(jsonConfig);
        return;
    }
}

void ConfigFile::updateLegacyConfig(var jsonConfig) {
    File legacyBackup = File(getHomePath() + "/.pocket-home/marshmallowConfig.json");
    legacyBackup.create();
    legacyBackup.replaceWithText(JSON::toString(jsonConfig));

    loadDefaultConfig();
    var pages = jsonConfig["pages"];
    if (pages.isArray()) {
        for (const var& page : *pages.getArray()) {
            if (page["name"] == "Apps") {
                favorites.clear();
            }
        }
    }
}

void ConfigFile::loadDefaultConfig() {

}