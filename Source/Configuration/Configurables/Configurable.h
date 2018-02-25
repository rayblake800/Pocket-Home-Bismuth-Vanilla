/**
 * @file Configurable.h
 * 
 * TODO: finish documentation.
 */
#pragma once
#include "../ConfigFile.h"
#include "../../../JuceLibraryCode/JuceHeader.h"

/**
 * Configurable objects set some of their properties based on user 
 * selection.  On creation, they register themselves with the a ConfigFile
 * object, indicating what data keys they track.  The ConfigFile then
 * notifies them any time that configuration data changes.
 */
class Configurable{
public:
    friend class ConfigFile;
    /**
     *Registers this object with the ConfigFile.
     * @param configFile a ConfigFile containing variables this object needs
     * to track.
     * @param trackedKeys the keys of all variables in configFile this
     * object will track.
     */
    Configurable(ConfigFile* configFile,Array<String> trackedKeys);
    /**
     *Unregisters this listener with the ConfigFile.
     */
    virtual ~Configurable();
protected:
    /**
     * Add to the list of keys tracked by this Configurable
     * @param newKeys
     */
    void addTrackedKeys(Array<String> newKeys);
    
    /**
     * Calls loadConfigProperties for every tracked key. This is the preferred
     * way to load configured properties on Configurable object initialization.
     */
    void loadAllConfigProperties();

private:
    /**
     * Reloads and applies object properties defined by a single key in
     * a configuration file
     * @param config the configFile containing the updated data value
     * @param key the key of property that has changed
     */
    virtual void loadConfigProperties(ConfigFile* config,String key) = 0;
    
    ScopedPointer<ConfigFile> config;
    Array<String> trackedKeys;
};
