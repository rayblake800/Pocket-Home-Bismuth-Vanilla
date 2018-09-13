#include "ConfigKey.h"

ConfigKey::ConfigKey(const char* keyString, const DataType dataType) :
key(juce::Identifier(keyString)), dataType(dataType) { }

ConfigKey::ConfigKey(const juce::Identifier& key, const DataType dataType) : 
key(key), dataType(dataType) { }

/*
 * Allow the ConfigKey to be used as if it was just its key value.
 */
ConfigKey::operator const juce::Identifier&() const
{
    return key;
}
