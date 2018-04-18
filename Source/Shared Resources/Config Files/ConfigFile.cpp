#include "AssetFiles.h"
#include "ConfigFile.h"

/**
 * @return a std::map of String keys mapped to integers, so getConfigValue()
 *          and setConfigValue() can use int as a template type. 
 */
template<> std::map<String, int>& ConfigFile::getMapReference<int>()
{
    return intValues;
}

/**
 * @return a std::map of String keys mapped to strings, so getConfigValue()
 *          and setConfigValue() can use String as a template type. 
 */
template<> std::map<String, String>& ConfigFile::getMapReference<String>()
{
    return stringValues;
}

/**
 * @return a std::map of String keys mapped to booleans, so getConfigValue()
 *          and setConfigValue() can use bool as a template type. 
 */
template<> std::map<String, bool>& ConfigFile::getMapReference<bool>()
{
    return boolValues;
}

ConfigFile::ConfigFile(String configFilename) : Localized("ConfigFile"),
filename(configFilename) { }

/**
 * Writes any pending changes to the file before destruction.
 */
ConfigFile::~ConfigFile()
{
    writeChanges();
}

/**
 * @return true iff this ConfigFile and rhs have the same filename.
 */
bool ConfigFile::operator==(const ConfigFile& rhs) const
{
    return filename == rhs.filename;
}

/**
 * Listeners safely remove themselves from all tracked ConfigFiles when
 * they are destroyed.
 */
ConfigFile::Listener::~Listener()
{
    const ScopedLock removalLock(configKeyAccess);
    while (configKeyMap.begin() != configKeyMap.end())
    {
        ConfigFile* trackedConfig = configKeyMap.begin()->first;
        const ScopedUnlock removeUnlock(configKeyAccess);
        trackedConfig->removeListener(this);
    }
}

/**
 * Calls configValueChanged() for every key tracked by this listener.
 */
void ConfigFile::Listener::loadAllConfigProperties()
{
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

/**
 * Adds a listener to the list of objects to notify when config values
 * change.
 */
void ConfigFile::addListener(ConfigFile::Listener* listener,
        StringArray trackedKeys)
{
    const ScopedLock keyMapLock(listener->configKeyAccess);
    listener->configKeyMap[this].mergeArray(trackedKeys,false);
    const ScopedUnlock keyMapUnlock(listener->configKeyAccess);
    for (const String& key : trackedKeys)
    {
        keyListeners[key].addIfNotAlreadyThere(listener);
    }
}

/**
 * Removes a listener from this ConfigFile.
 */
void ConfigFile::removeListener(ConfigFile::Listener* listener)
{
    
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

/**
 * Announce new changes to each object tracking a particular key.
 */
void ConfigFile::notifyListeners(String key)
{
    const ScopedLock lock(listenerLock);
    //since notifications are separated by key, and no variation exists between
    //key notifications, there's no need to make sure the queue is starting out
    //empty.
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

//################################# File IO ####################################

/**
 * Read in this object's data from a json config object
 */
void ConfigFile::readDataFromJson(var& config, var & defaultConfig)
{
    std::vector<DataKey> dataKeys = getDataKeys();
    for (const DataKey& key : dataKeys)
    {
        switch (key.dataType)
        {
            case stringType:
                initMapProperty<String>(key.keyString, getProperty
                                        (config, defaultConfig, key.keyString)
                                        .toString());
                break;

            case intType:
                initMapProperty<int>(key.keyString,
                                     getProperty(config, defaultConfig,
                                     key.keyString));
                break;

            case boolType:
                initMapProperty<bool>(key.keyString,
                                      getProperty(config, defaultConfig,
                                      key.keyString));

                break;
        }
    }
}

/**
 * Copy all config data to a json object
 */
void ConfigFile::copyDataToJson(DynamicObject::Ptr jsonObj)
{

    std::vector<DataKey> dataKeys = getDataKeys();
    for (const DataKey& key : dataKeys)
    {
        switch (key.dataType)
        {
            case stringType:
                jsonObj->setProperty(key.keyString,
                        getMapReference<String>()[key.keyString]);
                break;

            case intType:
                jsonObj->setProperty(key.keyString,
                        getMapReference<int>()[key.keyString]);
                break;

            case boolType:
                jsonObj->setProperty(key.keyString,
                        getMapReference<bool>()[key.keyString]);

                break;
        }
    }
}

/**
 * Checks if a property exists in a config data object loaded from a json
 * file.
 */
bool ConfigFile::propertyExists(var& config, String propertyKey)
{

    var property = config.getProperty(propertyKey, var());

    return !property.isVoid();
}

/**
 * Gets a property from json configuration data, or from default
 * configuration data if necessary
 */
var ConfigFile::getProperty(var& config, var& defaultConfig, String key)
{
    if (propertyExists(config, key))
    {
        return config.getProperty(key, var());
    }
    else
    {
        DBG("ConfigFile::" << __func__ << ": key " << key
                << " doesn't exist in " << filename);
        if (defaultConfig.isVoid())
        {
            defaultConfig = AssetFiles::loadJSONAsset
                    ("configuration/" + filename, false);
        }
        fileChangesPending = true;

        return defaultConfig.getProperty(key, var());
    }
}

/**
 * Marks this ConfigFile as containing changes that need to be written to
 * the underlying .json file.
 */
void ConfigFile::markPendingChanges()
{

    fileChangesPending = true;
}

/**
 * Re-writes all data back to the config file, as long as there are
 * changes to write.
 */
void ConfigFile::writeChanges()
{
    if (!fileChangesPending)
    {
        return;
    }
    DynamicObject::Ptr jsonBuilder = new DynamicObject();
    copyDataToJson(jsonBuilder.get());

    //convert to JSON string, write to config.json
    String jsonText = JSON::toString(jsonBuilder.get());
    File configFile = File(String(configPath) + filename);
    if (!configFile.exists())
    {
        configFile.create();
    }
    if (!configFile.replaceWithText(jsonText))
    {
        String message = localeText(failed_saving_to_FILE)
                + String(configPath) + filename + "\n"
                + localeText(check_permissions);
        AlertWindow::showMessageBox(
                AlertWindow::AlertIconType::WarningIcon,
                localeText(save_error),
                message);
    }
    else
    {
        fileChangesPending = false;
    }
}
