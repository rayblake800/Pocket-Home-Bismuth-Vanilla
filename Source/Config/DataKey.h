#pragma once
#include "Config.h"
#include "JuceHeader.h"

/**
 * @file  DataKey
 *
 * @brief  Holds the key string and data type of a value in a JSON
 *         configuration file.
 */

struct Config::DataKey
{
    /**
     * All valid key data types
     */
    enum DataType
    {
        stringType,
        intType,
        boolType,
        doubleType
    };
    
    /**
     * @param keyString  The key string identifying a value stored in a 
     *                   configuration file.
     *
     * @param dataType   The data type of that stored value
     */
    DataKey(const char* keyString, const DataType dataType);
    
    /**
     * @param key        The key identifying a value stored in a configuration 
     *                   file.
     *
     * @param dataType   The data type of that stored value
     */
    DataKey(const juce::Identifier& keyString, const DataType dataType);

    const juce::Identifier key;
    const DataType dataType;

    /**
     * Allow the DataKey to be used as if it was just its key value.
     */
    operator const juce::Identifier&() const;
};

