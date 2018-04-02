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
#include "gio/gio.h"

namespace GVariantConverter
{
    enum VariantType{
        booleanType = 0,
        uint32Type,
        stringType,
        stringArrayType,
        unsupported
    };
    
    VariantType getType(GVariant* variant);
    
    template<typename T> T getValue(GVariant* variant);
    
    template<typename T> GVariant* getVariant(T value);
};
