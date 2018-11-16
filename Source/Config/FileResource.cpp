#include "AssetFiles.h"
#include "FileResource.h"
    
/* The directory where all config files will be located. */
static const constexpr char* configPath = "~/.pocket-home/";

/* The pocket-home asset folder subdirectory containing default config files. */
static const constexpr char* defaultAssetPath = "configuration/";

/*
 * Creates the FileResource resource, and prepares to read JSON data.
 */
Config::FileResource::FileResource(
        const juce::Identifier& resourceKey,
        const juce::String& configFilename) :
SharedResource::Resource(resourceKey), 
filename(configFilename),
configJson(configPath + filename),
defaultJson(defaultAssetPath + filename) { }

/*
 * Writes any pending changes to the file before destruction.
 */
Config::FileResource::~FileResource()
{
    writeChanges();
}

/*
 * Sets a configuration data value back to its default setting.  If this
 * changes the value, listeners will be notified and changes will be saved.
 */
void Config::FileResource::restoreDefaultValue(const juce::Identifier& key)
{
    using namespace juce;
    //Check key validity, find expected data type
    const std::vector<DataKey>& keys = getConfigKeys();
    for(const DataKey& configKey : keys)
    {
        if(configKey.key == key)
        {
            restoreDefaultValue(configKey);
            return;
        }
    }
    DBG("FileResource::" << __func__ << ": Key \"" << key 
                << "\" is not expected in " << filename);
    throw BadKeyException(key);
}

/*
 * Restores all values in the configuration file to their defaults. All 
 * updated values will notify their Listeners and be written to the JSON
 * file.
 */
void Config::FileResource::restoreDefaultValues()
{
    using namespace juce;
    const std::vector<DataKey>& keys = getConfigKeys();
    for(const DataKey& key : keys)
    {
        restoreDefaultValue(key);
    }
    writeChanges();
}
/*
 * Calls configValueChanged() for every key tracked by this listener.
 */
void Config::FileResource::Listener::loadAllConfigProperties()
{
    using namespace juce;
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
 * Adds a key to the list of keys tracked by this listener.
 */
void Config::FileResource::Listener::addTrackedKey
(const juce::Identifier& keyToTrack)
{
    const juce::ScopedLock keyListLock(subscribedKeys.getLock());
    subscribedKeys.add(keyToTrack);
}

/*
 * Unsubscribes from updates to a FileResource value.
 */
void Config::FileResource::Listener::removeTrackedKey
(const juce::Identifier& keyToRemove)
{
    const juce::ScopedLock keyListLock(subscribedKeys.getLock());
    subscribedKeys.removeAllInstancesOf(keyToRemove);
}

/*
 * Announces a changed configuration value to each Listener object.
 */
void Config::FileResource::notifyListeners(const juce::Identifier& key)
{
    using namespace juce;
    foreachHandler<Listener>([this, &key](Listener* listener)
    {
        notifyListener(listener, key);
    });
}
    
/**
 * Checks if a single handler object is a Listener tracking updates of a single
 * key value, and if so, notifies it that the tracked value has updated.
 */
void Config::FileResource::notifyListener
(FileResource::Listener* listener, const juce::Identifier& key)
{
    using namespace juce;
    const ScopedLock trackedKeyLock(listener->subscribedKeys.getLock());
    if(listener->subscribedKeys.contains(key))
    {
        listener->configValueChanged(key);
    };
}

/*
 * Loads all initial configuration data from the JSON config file. This checks 
 * for all expected data keys, and replaces any missing or invalid values with 
 * ones from the default config file. FileResource subclasses should call this 
 * once, after they load any custom object or array data.
 */
void Config::FileResource::loadJSONData()
{
    using namespace juce;
    const std::vector<DataKey>& keys = getConfigKeys();
    for (const DataKey& key : keys)
    {
        try
        {
            switch (key.dataType)
            {
                case DataKey::stringType:
                    initProperty<String>(key);
                    break;
                case DataKey::intType:
                    initProperty<int>(key);
                    break;
                case DataKey::boolType:
                    initProperty<bool>(key);
                    break;
                case DataKey::doubleType:
                    initProperty<double>(key);
                    break;
                default:
                    DBG("FileResource::" << __func__ 
                            << ": Unexpected type for key \"" << key.key
                            << "\n in file " << filename);
            }
        }
        catch(JSONFile::FileException e)
        {
            DBG("FileResource::" << __func__ << ": " << e.what());
        }
        catch(JSONFile::TypeException e)
        {
            DBG("FileResource::" << __func__ << ": " << e.what());
        }
    }
    writeChanges();
}
 
/*
 * Checks if a key string is valid for this FileResource.
 */
bool Config::FileResource::isValidKey(const juce::Identifier& key) const
{
    const std::vector<DataKey>& keys = getConfigKeys();
    for(const DataKey& configKey : keys)
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
void Config::FileResource::writeChanges()
{
    using namespace juce;
    writeDataToJSON();
    try
    {
        configJson.writeChanges();
    }
    catch(JSONFile::FileException e)
    {
        DBG("FileResource::" << __func__ << ": " << e.what());
    }
    catch(JSONFile::TypeException e)
    {
        DBG("FileResource::" << __func__ << ": " << e.what());
    }
}

/**
 * Sets a configuration data value back to its default setting, notifying 
 * listeners if the value changes.
 */
void Config::FileResource::restoreDefaultValue(const DataKey& key)
{
    using namespace juce;
    try
    {
        switch(key.dataType)
        {
            case DataKey::stringType:
                setConfigValue<String>(key,
                        defaultJson.getProperty<String>(key));
                return;
            case DataKey::intType:
                setConfigValue<int>(key,
                        defaultJson.getProperty<int>(key));
                return;
            case DataKey::boolType:
                setConfigValue<bool>(key,
                        defaultJson.getProperty<bool>(key));
                return;
            case DataKey::doubleType:
                setConfigValue<double>(key,
                        defaultJson.getProperty<double>(key));
        }
    }
    catch(JSONFile::FileException e)
    {
        DBG("FileResource::" << __func__ << ": " << e.what());
    }
    catch(JSONFile::TypeException e)
    {
        DBG("FileResource::" << __func__ << ": " << e.what());
    }
}



