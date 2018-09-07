#include "ConfigKey.h"

ConfigKey::ConfigKey(const char* keyString, const DataType dataType) :
key(juce::Identifier(keyString)), dataType(dataType) { }

/*
 * Allow the ConfigKey to be used as if it was just its key value.
 */
ConfigKey::operator const juce::Identifier&() const
{
    return key;
}
