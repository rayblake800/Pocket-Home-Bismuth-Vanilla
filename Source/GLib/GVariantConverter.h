/**
 * @file GVariantConverter
 * 
 * @brief Provides convenience methods for working with GIo GVariant data.
 * 
 * This provides methods to convert different data types to and from the 
 * GVariant type used to interact with DBus objects.  Only the GVariant types
 * needed for this project will be supported.
 */

#pragma once
#include <functional>
#include "gio/gio.h"
#include "JuceHeader.h"

namespace GVariantConverter
{
    enum VariantType{
        booleanType = 0,
        byteType,
        int16Type,
        uint16Type,
        int32Type,
        uint32Type,
        int64Type,
        uint64Type,
        doubleType,
        stringType,
        byteStringType,
        arrayType,
        stringArrayType,
        dictType,
        unsupported
    };
    
    GVariant* unpack(GVariant* container);
    
    VariantType getType(GVariant* variant);
    
    GType getGType(GVariant* variant);
    
    GValue getGValue(GVariant* variant);
    
    String toString(GVariant* variant);
    
    template<typename T> T getValue(GVariant* variant);
    
    template<typename T> GVariant* getVariant(T value);
    
    void iterateArray(GVariant* array,
            std::function<void(GVariant*)> arrayCall);
    
    void iterateDict(GVariant* dict,
            std::function<void(GVariant*, GVariant*)> dictCall);
    
    StringArray getKeys(GVariant* dict);
    
};
