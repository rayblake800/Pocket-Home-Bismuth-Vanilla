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
 * Add to the list of keys tracked by this Configurable
 * @param newKeys
 */
void Configurable::addTrackedKeys(Array<String> newKeys) {
    config->registerConfigurable(this, newKeys);
    trackedKeys.addArray(newKeys);
}

/**
 * Calls loadConfigProperties for every tracked key.
 */
void Configurable::loadAllConfigProperties()
{
    for (const String& key : trackedKeys)
    {
        loadConfigProperties(config, key);
    }
}
