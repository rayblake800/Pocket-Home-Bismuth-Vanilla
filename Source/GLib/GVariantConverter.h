/**
 * @file GVariantConverter
 * 
 * @brief Provides convenience methods for working with GIO GVariant data.
 * 
 * This provides methods to convert different data types to and from the 
 * GVariant type used to interact with DBus objects.  Only the GVariant types
 * needed for this project will be supported.
 */

#pragma once
#include <functional>
#include <map>
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
    
    /**
     * If a GVariant is a container holding a single item, remove that item
     * from the container and return it as a GVariant*.  This function will not
     * dereference the container object.
     * 
     * @param container  the GVariant object to try and unpack.
     * 
     * @return  The item extracted from container, or nullptr if container is
     *          not a GVariant container with a single item.  Any non-null
     *          value returned by this function will need to be freed with
     *          g_variant_unref();
     */
    GVariant* unpack(GVariant* container);
    
    /**
     * Extract the value from a GVariant object as a specific type.  Supported
     * types are bool, guint32, String, StringArray, GArray*, and GByteArray*.
     * 
     * @tparam T       The value type to extract from the variant.  If this
     *                 does not match the variant type, a default value will
     *                 be returned.
     * 
     * @param variant  A GVariant object storing data of type T.
     * 
     * @return  the extracted data, or a default value if no value could be
     *          extracted.  
     *          Default values:
     *          bool        : false
     *          guint32     : 0
     *          String      : ""
     *          StringArray : {}
     *          GArray*     : nullptr
     *          GByteArray* : nullptr
     */
    template<typename T> T getValue(GVariant* variant);
    
    /**
     * Packages a variable into a GVariant* object.  Supported types are bool,
     * guint32, String, and StringArray.
     * 
     * @param value  Any variable with a supported type.
     * 
     * @return  the newly created GVariant.  When no longer needed, this should
     *          be freed with g_variant_unref().
     */
    template<typename T> GVariant* getVariant(T value);
    
    /**
     * Get the VariantType enum value that best describes a variant.
     * 
     * @param variant  The GVariant object to check.
     * 
     * @return  the VariantType value that best describes the GVariant object.
     */
    VariantType getType(GVariant* variant);
    
    /**
     * Get the GType that best fits the data stored in a GVariant object.  This
     * only supports a small subset of GTypes.
     * 
     * @param variant  The GVariant object to check.
     * 
     * @return the GType of the variant stored value, or G_TYPE_INVALID if
     *         the variant is null or has an unsupported type.
     */
    GType getGType(GVariant* variant);
    
    /**
     * Attempts to extract a value from a GVariant object and store it within a
     * GValue object.  
     * 
     * @param variant  The GVariant to extract data from.
     * 
     * @return   the GValue containing the stored data, or G_VALUE_INIT if the
     *           value could not be transfered. 
     */
    GValue getGValue(GVariant* variant);
    
    /**
     * Convert any GVariant object into a formatted string object.
     * 
     * @param variant  the GVariant to convert
     * 
     * @return  the formatted string 
     */
    String toString(GVariant* variant);
    
    /**
     * Iterate through a GVariant* container, running a callback function for
     * each array element.
     * 
     * @param array      A GVariant container object.
     *  
     * @param arrayCall  This function will run once for each element in the
     *                   array, passing in the array element as a parameter.
     *                   The array elements passed in will be dereferenced
     *                   after the function runs.
     */
    void iterateArray(GVariant* array,
            std::function<void(GVariant*)> arrayCall);
    
    /**
     * Iterate through a GVariant* dictionary, running a callback function for
     * each key/value pair.
     * 
     * @param dict      A GVariant dictionary object.
     * 
     * @param dictCall  This function will run once for each key/value pair in
     *                  the dictionary. Each call passes in a single key and its
     *                  value as GVariant* objects. These objects will be
     *                  dereferenced after the function runs.
     */
    void iterateDict(GVariant* dict,
            std::function<void(GVariant*, GVariant*)> dictCall);
    
    /**
     * Extract all key strings from a GVariant* dictionary.
     * 
     * @param dict  A GVariant dictionary object.
     * 
     * @return  the extracted key strings, or an empty array if dict was not a
     *          valid dictionary.
     */
    StringArray getKeys(GVariant* dict);
    
};
