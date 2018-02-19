#include "../Utils.h"
#include "ConfigFile.h"
#include "Configurables/Configurable.h"

/**
 *TODO: There must be a better way to manage types than to have near-identical
 * getter/setter methods for each type.  See if there's some template trick
 * to make this a little less repetitive.
 */

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

//######################### Integer Data ###################################

/**
 * Gets one of the integer values stored in the json configuration file
 */
int ConfigFile::getConfigInt(String intKey)
{
    const ScopedLock readLock(lock);
    try
    {
        return intValues.at(intKey);
    } catch (std::out_of_range e)
    {
        DBG(String("getConfigInt: key \"") + intKey
                + String("\" is not an integer value stored in ") + filename);
        throw e;
    }
}

/**
 * Sets one of this ConfigFile's integer values, writing it to the config 
 * file if the value has changed.  
 */
void ConfigFile::setConfigInt(String intKey, int newValue)
{
    const ScopedLock writeLock(lock);
    try
    {
        if (intValues.at(intKey) != newValue)
        {
            changesPending = true;
            intValues[intKey] = newValue;
            writeChanges();

            const ScopedUnlock allowDataAccess(lock);
            notifyConfigurables(intKey);
        }
    } catch (std::out_of_range e)
    {
        DBG(String("setConfigInt: key ") + intKey
                + String(" is not a string value stored in ") + filename);
        throw e;
    }
}

//############################ String Data #####################################

/**
 * Gets one of the string values stored in the json configuration file
 */
String ConfigFile::getConfigString(String stringKey)
{
    const ScopedLock readLock(lock);
    try
    {
        return stringValues.at(stringKey);
    } catch (std::out_of_range e)
    {
        DBG(String("getConfigString: key \"") + stringKey
                + String("\" is not a string value stored in ") + filename);
        throw e;
    }
}

/**
 * Sets one of this ConfigFile's string values, writing it to the config 
 * file if the value has changed.  
 */
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
        DBG(String("setConfigString: key ") + stringKey
                + String(" is not a string value stored in ") + filename);
        throw e;
    }
}

//############################ Boolean Data ####################################

/**
 * Gets one of the boolean values stored in the json configuration file.
 */
bool ConfigFile::getConfigBool(String boolKey)
{
    const ScopedLock readLock(lock);
    try
    {
        return boolValues.at(boolKey);
    } catch (std::out_of_range e)
    {
        DBG(String("getConfigBool: key ") + boolKey
                + String(" is not a boolean value stored in ") + filename);
        throw e;
    }

}

/**
 * Sets one of this ConfigFile's boolean values, writing it to the config 
 * file if the value has changed.  
 */
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
        DBG(String("setConfigBool: key ") + boolKey
                + String(" is not a boolean value stored in ") + filename);
        throw e;
    }
}

/**
 * @return true iff this ConfigFile and rhs have the same filename.
 */
bool ConfigFile::operator==(const ConfigFile& rhs) const
{
    return filename == rhs.filename;
}

/**
 * @return the keys to all integer variables tracked in this config file.
 */
Array<String> ConfigFile::getIntKeys() const
{
    return {};
}

/**
 * @return the keys to all string variables tracked in this config file.
 */
Array<String> ConfigFile::getStringKeys() const
{
    return {};
}

/**
 * @return the keys to all boolean variables tracked in this config file.
 */
Array<String> ConfigFile::getBoolKeys() const
{
    return {};
}
//################################# File IO ####################################

/**
 * Read in this object's data from a json config object
 */
void ConfigFile::readDataFromJson(var& config, var& defaultConfig)
{
    //load integers
    Array<String> intKeys = getIntKeys();
    for (const String& key : intKeys)
    {
        intValues[key] = getProperty(config, defaultConfig, key);
    }
    
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
    Array<String> intKeys = getIntKeys();
    for (const String& key : intKeys)
    {
        jsonObj->setProperty(key, intValues[key]);
    }
    
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
 * Checks if a property exists in a config data object loaded from a json
 * file.
 */
bool ConfigFile::propertyExists(var& config, String propertyKey)
{
    var property = config.getProperty(propertyKey, var::null);
    return var::null != property;
}

/**
 * Gets a property from json configuration data, or from default
 * configuration data if necessary
 */
var ConfigFile::getProperty(var& config, var& defaultConfig, String key)
{
    bool exists = propertyExists(config, key);
    DBG(key + (exists ? String(" exists") : String(" doesn't exist")));
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