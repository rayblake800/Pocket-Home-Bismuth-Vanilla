#include "Configurable.h"

/**
 *Registers this object with the ConfigFile.
 */
Configurable::Configurable(ConfigFile* configFile, Array<String> trackedKeys) :
config(configFile),
trackedKeys(trackedKeys)
{
    config->registerConfigurable(this, trackedKeys);
}

/**
 *Unregisters this listener with the ConfigFile.
 */
Configurable::~Configurable()
{
    config->unregisterConfigurable(this, trackedKeys);
}

/**
 * Calls loadConfigProperties for every tracked key.
 */
void Configurable::loadAllConfigProperties(){
    for(const String& key : trackedKeys){
        loadConfigProperties(config,key);
    }
}

/**
 *Receives notifications whenever configuration changes occur. 
 */
void Configurable::configChanged(String key)
{
    loadConfigProperties(config, key);
}