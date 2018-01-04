/*
  ==============================================================================

    Configurable.cpp
    Created: 1 Jan 2018 12:18:07am
    Author:  anthony

  ==============================================================================
 */

#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent(ComponentType componentType,
            Array<ConfigString> trackedStrings,
            Array<ConfigBool> trackedBools){
}

ConfigurableComponent::~ConfigurableComponent() {
}

void ConfigurableComponent::changeListenerCallback(ChangeBroadcaster * source){
    if(changesAreRelevant()){
        loadConfigProperties();
    }
}