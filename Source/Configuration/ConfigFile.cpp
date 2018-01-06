/*
  ==============================================================================

    ConfigFile.cpp
    Created: 26 Dec 2017 10:31:17am
    Author:  anthony
  ==============================================================================
 */


#include "../Utils.h"
#include "ConfigFile.h"
#include "Configurable.h"

ConfigFile::ConfigFile(String configFilename) : filename(configFilename)
{
}

/**
 * Writes any pending changes to the file before destruction.
 */
ConfigFile::~ConfigFile()
{
    const ScopedLock writeLock(lock);
    writeChanges();
}

/**
 * Register an object as tracking configuration changes. That object
 * is notified whenever any data it tracks is changed.
 * 
 * @param configurable an object that uses data stored in this ConfigFile 
 * @param keys all data keys that the object needs to track.
 */
void ConfigFile::registerConfigurable(Configurable * configurable,
        Array<String> keys)
{
    const ScopedLock changeLock(lock);
    for (const String& key : keys)
    {
        configured[key].add(configurable);
    }
}

/**
 * Removes an object from the list of objects to notify when configuration
 * changes.
 * @param configurable a configurable object to be unregistered.  If the
 * object wasn't actually registered, nothing will happen.
 * @param keys all keys that the configurable object will no longer track.
 */
void ConfigFile::unregisterConfigurable(Configurable * configurable,
        Array<String> keys)
{
    const ScopedLock changeLock(lock);
    for (const String& key : keys)
    {
        configured[key].removeAllInstancesOf(configurable);
    }
}

//############################ String Data #####################################

String ConfigFile::getConfigString(String stringKey)
{
    const ScopedLock readLock(lock);
    try
    {
        return stringValues.at(stringKey);
    } catch (std::out_of_range e)
    {
        DBG("getConfigString:missing key value!");
        return "";
    }
}

void ConfigFile::setConfigString(String stringKey,
        String newValue)
{
    const ScopedLock writeLock(lock);
    try
    {
        if (stringValues.at(stringKey) != newValue)
        {
            changesPending = true;
            stringValues[stringKey] = newValue;
            writeChanges();

            const ScopedUnlock allowDataAccess(lock);
            notifyConfigurables(stringKey);
        }
    } catch (std::out_of_range e)
    {
        DBG("setConfigString:missing key value!");
    }
}

//############################ Boolean Data ####################################

bool ConfigFile::getConfigBool(String boolKey)
{
    const ScopedLock readLock(lock);
    try
    {
        return boolValues.at(boolKey);
    } catch (std::out_of_range e)
    {
        DBG("setConfigBool:missing key value!");
        return false;
    }

}

void ConfigFile::setConfigBool(String boolKey, bool newValue)
{
    const ScopedLock writeLock(lock);
    try
    {
        if (boolValues.at(boolKey) != newValue)
        {
            changesPending = true;
            boolValues[boolKey] = newValue;
            writeChanges();
            const ScopedUnlock allowDataAccess(lock);
            notifyConfigurables(boolKey);

        }
    } catch (std::out_of_range e)
    {
        DBG("setConfigBool:missing key value!");
    }
}



//################################# File IO ####################################

/**
 * Read in this object's data from a json config object
 */
void ConfigFile::readDataFromJson(var& config, var& defaultConfig)
{
    //load strings
    Array<String> stringKeys = getStringKeys();
    for (const String& key : stringKeys)
    {
        stringValues[key] = getProperty(config, defaultConfig, key).toString();
    }

    //load bools
    Array<String> boolKeys = getBoolKeys();
    for (const String& key : boolKeys)
    {
        boolValues[key] = getProperty(config, defaultConfig, key);
    }

}

/**
 * Copy all config data to a json object
 */
void ConfigFile::copyDataToJson(DynamicObject::Ptr jsonObj)
{
    Array<String> stringKeys = getStringKeys();
    for (const String& key : stringKeys)
    {
        jsonObj->setProperty(key, stringValues[key]);
    }

    Array<String> boolKeys = getBoolKeys();
    for (const String& key : boolKeys)
    {
        jsonObj->setProperty(key, boolValues[key]);
    }
}

/**
 * Checks if a property exists in config data
 */
bool ConfigFile::propertyExists(var& config, String propertyKey)
{
    var property = config.getProperty(propertyKey,var::null);
    return var::null != property;
}

/**
 * Gets a property from json configuration data, or from default
 * configuration data if necessary
 */
var ConfigFile::getProperty(var& config, var& defaultConfig, String key)
{
    bool exists=propertyExists(config, key);
    DBG(key+(exists?String("exists"):String("doesn't exist")));
    if (propertyExists(config, key))
    {
        return config.getProperty(key, var::null);
    } else
    {
        if (defaultConfig == var::null)
        {
            defaultConfig = JSON::parse(assetFile(filename));
        }
        changesPending = true;
        return defaultConfig.getProperty(key, var::null);
    }
}

/**
 * Re-writes all data back to the config file, as long as there are
 * changes to write.
 */
void ConfigFile::writeChanges()
{
    if (!changesPending)
    {
        return;
    }
    DynamicObject::Ptr jsonBuilder = new DynamicObject();
    copyDataToJson(jsonBuilder.get());

    //convert to JSON string, write to config.json
    String jsonText = JSON::toString(jsonBuilder.get());
    File configFile = File(getHomePath() + String(CONFIG_PATH) + filename);
    if (!configFile.exists())
    {
        configFile.create();
    }
    if (!configFile.replaceWithText(jsonText))
    {
        String message = String("Failed to save changes to ~")
                + String(CONFIG_PATH) + filename
                + String("\nMake sure you have permission to write to this file.");
        AlertWindow::showMessageBox(
                AlertWindow::AlertIconType::WarningIcon,
                "Error saving configuration:",
                message);
    } else
    {
        changesPending = false;
    }
}

/**
 * Announce new changes to each object tracking a particular key.
 */
void ConfigFile::notifyConfigurables(String key)
{
    for (Configurable * tracking : configured[key])
    {
        tracking->configChanged(key);
    }
}