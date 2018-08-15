#include "AssetFiles.h"
#include "ConfigFile.h"


/*
 * This constructor should only be called when constructing ConfigFile
 * subclasses.
 */
ConfigFile::ConfigFile(juce::String configFilename) :
filename(configFilename),
configJson(configPath + filename),
defaultJson(defaultAssetPath + filename) { }


/*
 * Writes any pending changes to the file before destruction.
 */
ConfigFile::~ConfigFile()
{
    writeChanges();
}


/*
 * Listeners safely remove themselves from all tracked ConfigFiles when
 * they are destroyed.
 */
ConfigFile::Listener::~Listener()
{
    using namespace juce;
    const ScopedLock removalLock(configKeyAccess);
    while (configKeyMap.begin() != configKeyMap.end())
    {
        ConfigFile* trackedConfig = configKeyMap.begin()->first;
        const ScopedUnlock removeUnlock(configKeyAccess);
        trackedConfig->removeListener(this);
    }
}

/*
 * Calls configValueChanged() for every key tracked by this listener.
 */
void ConfigFile::Listener::loadAllConfigProperties()
{
    using namespace juce;
    const ScopedLock updateLock(configKeyAccess);
    for (auto it = configKeyMap.begin(); it != configKeyMap.end(); it++)
    {
        for (const String& key : it->second)
        {
            const ScopedUnlock notifyUnlock(configKeyAccess);
            configValueChanged(key);
        }
    }
}

/*
 * Adds a listener to the list of objects to notify when config values
 * change.
 */
void ConfigFile::addListener(ConfigFile::Listener* listener,
        juce::StringArray trackedKeys)
{
    using namespace juce;
    const ScopedLock keyMapLock(listener->configKeyAccess);
    listener->configKeyMap[this].mergeArray(trackedKeys,false);
    const ScopedUnlock keyMapUnlock(listener->configKeyAccess);
    for (const String& key : trackedKeys)
    {
        keyListeners[key].addIfNotAlreadyThere(listener);
    }
}

/*
 * Removes a listener from this ConfigFile.
 */
void ConfigFile::removeListener(ConfigFile::Listener* listener)
{
    using namespace juce;
    const ScopedLock keyMapLock(listener->configKeyAccess);
    StringArray trackedKeys = listener->configKeyMap[this];
    listener->configKeyMap.erase(this);
    const ScopedUnlock keyMapUnlock(listener->configKeyAccess);
    const ScopedLock lock(listenerLock);
    for (const String& key : trackedKeys)
    {
        keyListeners[key].removeAllInstancesOf(listener);
        notificationQueue[key].removeAllInstancesOf(listener);
    }
}

/*
 * Announce new changes to each object tracking a particular key.
 */
void ConfigFile::notifyListeners(juce::String key)
{
    using namespace juce;
    const ScopedLock lock(listenerLock);
    
    /* In other Listener implementations, it's necessary to wait until the queue
     * is empty, to prevent this notification event from interfering with
     * others. Since these notifications are separated by key, that's not
     * necessary here.
     */ 
    notificationQueue[key].addArray(keyListeners[key]);
    while(!notificationQueue[key].isEmpty())
    {
        Listener* toNotify = notificationQueue[key].removeAndReturn(
                notificationQueue[key].size() - 1);
        const ScopedUnlock signalUnlock(listenerLock);
        const MessageManagerLock mmLock;
        toNotify->configValueChanged(key);
    }
}

/*
 * Loads all initial configuration data from the JSON config file. This 
 * checks for all expected data keys, and replaces any missing or invalid
 * values with ones from the default config file. ConfigFile subclasses
 * should call this once, after they load any custom object or array data.
 */
void ConfigFile::loadJSONData()
{
    using namespace juce;
    const std::vector<DataKey>& dataKeys = getDataKeys();
    for (const DataKey& key : dataKeys)
    {
        try
        {
            switch (key.dataType)
            {
                case stringType:
                    initProperty<String>(key.keyString);
                    break;
                case intType:
                    initProperty<int>(key.keyString);
                    break;
                case boolType:
                    initProperty<bool>(key.keyString);
                    break;
                case doubleType:
                    initProperty<double>(key.keyString);
                    break;
                default:
                    DBG("ConfigFile::" << __func__ 
                            << ": Unexpected type for key \"" << key.keyString
                            << "\n in file " << filename);
            }
        }
        catch(JSONFile::FileException e)
        {
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
    }
    writeChanges();
}

/*
 * Sets a configuration data value back to its default setting.  If this
 * changes the value, listeners will be notified and changes will be saved.
 */
void ConfigFile::restoreDefaultValue(juce::String key)
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
        DBG("ConfigFile::" << __func__ << ": Key \"" << key 
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
        DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
    catch(JSONFile::TypeException e)
    {
        DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
}

/*
 * Restores all values in the configuration file to their defaults. All 
 * updated values will notify their Listeners and be written to the JSON
 * file.
 */
void ConfigFile::restoreDefaultValues()
{
    using namespace juce;
    const std::vector<DataKey>& keys = getDataKeys();
    for(const DataKey& key : keys)
    {
        try
        {
            switch(key.dataType)
            {
                case stringType:
                    setConfigValue<String>(key.keyString,
                            defaultJson.getProperty<String>(key.keyString));
                    break;
                case intType:
                    setConfigValue<int>(key.keyString,
                            defaultJson.getProperty<int>(key.keyString));
                    break;
                case boolType:
                    setConfigValue<bool>(key.keyString,
                            defaultJson.getProperty<bool>(key.keyString));
                    break;
                case doubleType:
                    setConfigValue<double>(key.keyString,
                            defaultJson.getProperty<double>(key.keyString));
                    break;
            }
        }
        catch(JSONFile::FileException e)
        {
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
            alertWindows.showPlaceholderError(e.getErrorMessage());
        }
    }
    writeChanges();
}
 
/*
 * Checks if a key string is valid for this ConfigFile.
 */
bool ConfigFile::isValidKey(const juce::String& key) const
{
    const std::vector<DataKey>& dataKeys = getDataKeys();
    for(const DataKey& dataKey : dataKeys)
    {
        if(dataKey.keyString == key)
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
void ConfigFile::writeChanges()
{
    using namespace juce;
    writeDataToJSON();
    try
    {
        configJson.writeChanges();
    }
    catch(JSONFile::FileException e)
    {
        DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
    catch(JSONFile::TypeException e)
    {
        DBG("ConfigFile::" << __func__ << ": " << e.getErrorMessage());
        alertWindows.showPlaceholderError(e.getErrorMessage());
    }
}
