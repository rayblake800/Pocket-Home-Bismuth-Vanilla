#include "AssetFiles.h"
#include "ConfigJSON.h"


/*
 * This constructor should only be called when constructing ConfigJSON
 * subclasses.
 */
ConfigJSON::ConfigJSON(
        const juce::Identifier& resourceKey,
        const juce::String& configFilename) :
SharedResource(resourceKey), 
filename(configFilename),
configJson(configPath + filename),
defaultJson(defaultAssetPath + filename) { }


/*
 * Writes any pending changes to the file before destruction.
 */
ConfigJSON::~ConfigJSON()
{
    writeChanges();
}

/*
 * Sets a configuration data value back to its default setting.  If this
 * changes the value, listeners will be notified and changes will be saved.
 */
void ConfigJSON::restoreDefaultValue(const juce::Identifier& key)
{
    using namespace juce;
    //Check key validity, find expected data type
    SupportedDataType valueType; 
    const std::vector<DataKey>& keys = getDataKeys();
    bool keyFound = false;
    for(const DataKey& dataKey : keys)
    {
        if(dataKey.keyString == key)
        {
            keyFound = true;
            valueType = dataKey.dataType;
            break;
        }
    }
    if(!keyFound)
    {
        DBG("ConfigJSON::" << __func__ << ": Key \"" << key 
                << "\" is not expected in " << filename);
        throw BadKeyException(key);
    }
    try
    {
        switch (valueType)
        {
            case stringType:
                setConfigValue<String>(key,
                        defaultJson.getProperty<String>(key));
                return;
            case intType:
                setConfigValue<int>(key,
                        defaultJson.getProperty<int>(key));
                return;
            case boolType:
                setConfigValue<bool>(key,
                        defaultJson.getProperty<bool>(key));
                return;
            case doubleType:
                setConfigValue<double>(key,
                        defaultJson.getProperty<double>(key));
        }
    }
    catch(JSONFile::FileException e)
    {
        DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
    catch(JSONFile::TypeException e)
    {
        DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
}

/*
 * Restores all values in the configuration file to their defaults. All 
 * updated values will notify their Listeners and be written to the JSON
 * file.
 */
void ConfigJSON::restoreDefaultValues()
{
    using namespace juce;
    const std::vector<ConfigKey>& keys = getConfigKeys();
    for(const ConfigKey& key : keys)
    {
        try
        {
            switch(key.dataType)
            {
                case ConfigKey::stringType:
                    setConfigValue<String>(key,
                            defaultJson.getProperty<String>(key));
                    break;
                case ConfigKey::intType:
                    setConfigValue<int>(key,
                            defaultJson.getProperty<int>(key));
                    break;
                case ConfigKey::boolType:
                    setConfigValue<bool>(key,
                            defaultJson.getProperty<bool>(key));
                    break;
                case ConfigKey::doubleType:
                    setConfigValue<double>(key,
                            defaultJson.getProperty<double>(key));
                    break;
            }
        }
        catch(JSONFile::FileException e)
        {
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
    }
    writeChanges();
}

ConfigJSON::Listener::Listener(
        const juce::Identifier& configJSONKey,
        const std::function<SharedResource*()> initJSON) :
ResourceHandler<ConfigJSON>(configJSONKey, initJSON) { }


/*
 * Calls configValueChanged() for every key tracked by this listener.
 */
void ConfigJSON::Listener::loadAllConfigProperties()
{
    using namespace juce;
    //Copy keys in case the key list changes during configValueChanged() calls.
    Array<Identifier> notifyKeys;
    {
        const ScopedLock updateLock(subscribedKeys.getLock());
        notifyKeys.addArray(subscribedKeys);
    }
    for (const Identifier& key : notifyKeys)
    {
       configValueChanged(key);
    }
}
/*
 * Subscribe to receive updates on a ConfigJSON value.
 */
void ConfigJSON::Listener::subscribeToKey
(const juce::Identifier& keyToTrack)
{
    using namespace juce;
    const ScopedLock keyListLock(subscribedKeys.getLock());
    subscribedKeys.add(keyToTrack);
}

/*
 * Unsubscribe from updates on a ConfigJSON value.
 */
void ConfigJSON::Listener::unsubscribeFromKey
(const juce::Identifier& keyToRemove)
{
    using namespace juce;
    const ScopedLock keyListLock(subscribedKeys.getLock());
    subscribedKeys.removeAllInstancesOf(keyToRemove);
}


/*
 * Announce new changes to each object tracking a particular key.
 */
void ConfigJSON::notifyListeners(const juce::Identifier& key)
{
    using namespace juce;
    foreachHandler([this, &key](Handler* handler)
    {
        Listener* toNotify = dynamic_cast<Listener*>(handler);
        if(toNotify != nullptr)
        {
            toNotify->configValueChanged(key);
        }
    });
}

/*
 * Loads all initial configuration data from the JSON config file. This 
 * checks for all expected data keys, and replaces any missing or invalid
 * values with ones from the default config file. ConfigJSON subclasses
 * should call this once, after they load any custom object or array data.
 */
void ConfigJSON::loadJSONData()
{
    using namespace juce;
    const std::vector<ConfigKey>& keys = getConfigKeys();
    for (const ConfigKey& key : keys)
    {
        try
        {
            switch (key.dataType)
            {
                case ConfigKey::stringType:
                    initProperty<String>(key);
                    break;
                case ConfigKey::intType:
                    initProperty<int>(key);
                    break;
                case ConfigKey::boolType:
                    initProperty<bool>(key);
                    break;
                case ConfigKey::doubleType:
                    initProperty<double>(key);
                    break;
                default:
                    DBG("ConfigJSON::" << __func__ 
                            << ": Unexpected type for key \"" << key.key
                            << "\n in file " << filename);
            }
        }
        catch(JSONFile::FileException e)
        {
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
    }
    writeChanges();
}
 
/*
 * Checks if a key string is valid for this ConfigJSON.
 */
bool ConfigJSON::isValidKey(const juce::Identifier& key) const
{
    const std::vector<ConfigKey>& keys = getConfigKeys();
    for(const ConfigKey& configKey : keys)
    {
        if(configKey.key == key)
        {
            return true;
        }
    }
    return false;
}

/*
 * Re-writes all data back to the config file, as long as there are
 * changes to write.
 */
void ConfigJSON::writeChanges()
{
    using namespace juce;
    writeDataToJSON();
    try
    {
        configJson.writeChanges();
    }
    catch(JSONFile::FileException e)
    {
        DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
    catch(JSONFile::TypeException e)
    {
        DBG("ConfigJSON::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
}
