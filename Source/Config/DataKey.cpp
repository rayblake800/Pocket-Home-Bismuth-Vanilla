#include "DataKey.h"

Config::DataKey::DataKey(const char* keyString, const DataType dataType) :
key(juce::Identifier(keyString)), dataType(dataType) { }

Config::DataKey::DataKey(const juce::Identifier& key, const DataType dataType) : 
key(key), dataType(dataType) { }

/*
 * Allow the DataKey to be used as if it was just its key value.
 */
Config::DataKey::operator const juce::Identifier&() const
{
    return key;
}
