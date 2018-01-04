/*
  ==============================================================================

    Configurable.h
    Created: 1 Jan 2018 12:18:07am
    Author:  anthony

  ==============================================================================
 */

#pragma once

#include "ConfigFile.h"
#include "../JuceLibraryCode/JuceHeader.h"

/**
 *ConfigurableComponent objects set some of their properties based on user 
 * selection.  On creation, they register themselves with the global ConfigFile
 * object, indicating what configuration data they track.  The ConfigFile then
 * notifies them any time that configuration data changes.
 * 
 */
class ConfigurableComponent : public ChangeListener {
public:
    /**
     *Registers this component with the ConfigFile.
     * 
     */
    ConfigurableComponent(ComponentType componentType,
            Array<ConfigString> trackedStrings,
            Array<ConfigBool> trackedBools);
    /**
     *Unregisters this listener with the broadcaster.
     */
    virtual ~ConfigurableComponent();
protected:
    /**
     *Reloads and applies all object properties defined in config.json
     */
    virtual void loadConfigProperties() = 0;

private:
    /**
     *Checks to see if configuration changes affect this object,
     * returning true if they do.
     */
    virtual bool changesAreRelevant() = 0;

    /**
     *Receives notifications whenever configuration changes occur.
     */
    void changeListenerCallback(ChangeBroadcaster * source) override;

};
