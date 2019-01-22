#include "GLib_VariantConverter.h"
#include "JuceHeader.h"

/*
 * Removes and returns a GVariant* value from a GVariant* container if and 
 * only if that value was the only item in the container.
 */
GVariant* GLib::VariantConverter::unpack(GVariant* container)
{
    if (g_variant_is_container(container)
        && g_variant_n_children(container) == 1)
    {
        GVariantIter arrayIter;
        g_variant_iter_init(&arrayIter, container);
        GVariant * child = nullptr;
        if ((child = g_variant_iter_next_value(&arrayIter)))
        {
            return child;
        }
    }
    return nullptr;
}

/*
 * Extracts the value from a GVariant object as a specific type.  Supported
 * types are bool, guint32, String, StringArray, GArray*, and GByteArray*.
 */
template<> bool GLib::VariantConverter::getValue(GVariant* variant)
{
    if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_BOOLEAN))
    {
        jassertfalse;
        return false;
    }
    return g_variant_get_boolean(variant);
}

template<> guint32 GLib::VariantConverter::getValue(GVariant* variant)
{
    if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_UINT32))
    {
        jassertfalse;
        return 0;
    }
    return g_variant_get_int32(variant);
}

template<> const char* GLib::VariantConverter::getValue(GVariant* variant)
{
    if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING))
    {
        jassertfalse;
        return nullptr;
    }
    return g_variant_get_string(variant, nullptr);
}

template<> juce::String GLib::VariantConverter::getValue(GVariant* variant)
{
    if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING))
    {
        jassertfalse;
        return "";
    }
    return g_variant_get_string(variant, nullptr);
}

template<> juce::StringArray GLib::VariantConverter::getValue(GVariant* variant)
{
    const gchar ** array = nullptr;
    gsize arraySize = 0;
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING_ARRAY))
    {
        array = g_variant_get_strv(variant, &arraySize);
    }
    else if (g_variant_is_of_type(variant,
            G_VARIANT_TYPE_OBJECT_PATH_ARRAY))
    {
        array = g_variant_get_objv(variant, &arraySize);
    }
    else
    {
        jassertfalse;
        return {};
    }
    juce::StringArray varArray;
    for (int i = 0; i < arraySize; i++)
    {
        varArray.add(juce::String(array[i]));
    }
    g_free(array);
    return varArray;
}

template<> GArray* GLib::VariantConverter::getValue(GVariant* variant)
{
    if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_ARRAY))
    {
        jassertfalse;
        return nullptr;
    }
    gsize numChildren = g_variant_n_children(variant);
    if (numChildren == 0)
    {
        return nullptr;
    }
    gsize childSize = g_variant_get_size(variant) / numChildren;

    gconstpointer arrayData = g_variant_get_fixed_array(variant,
            &numChildren, childSize);
    GArray* arrayValue = g_array_sized_new(false, true,
            g_variant_get_size(variant) / numChildren, numChildren);
    g_array_append_vals(arrayValue, arrayData, numChildren);
    return arrayValue;
}

template<> GByteArray* GLib::VariantConverter::getValue(GVariant* variant)
{
    GByteArray* array = g_byte_array_new();
    gsize numBytes = 0;
    gconstpointer byteData = g_variant_get_fixed_array(variant,
            &numBytes, sizeof(guchar));
    g_byte_array_append(array, (const guint8*) byteData, numBytes);
    return array;
}

template<> juce::uint64 GLib::VariantConverter::getValue(GVariant* variant)
{
     if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_UINT64))
    {
        jassertfalse;
        return -1;
}
    return g_variant_get_uint64(variant);
}

/*
 * Packages a variable into a GVariant* object.  Supported types are bool,
 * guint32, String, and StringArray.
 */
template<> GVariant* GLib::VariantConverter::getVariant(bool value)
{
    return g_variant_new_boolean(value);
}

template<> GVariant* GLib::VariantConverter::getVariant(guint32 value)
{
    return g_variant_new_uint32(value);
}

template<> GVariant* GLib::VariantConverter::getVariant(juce::String value)
{
    return g_variant_new_string(value.toRawUTF8());
}

template<> GVariant* GLib::VariantConverter::getVariant(juce::StringArray value)
{
    const char ** array = new (std::nothrow) const char*[value.size()];
    if (array == nullptr)
    {
        DBG("GLib::VariantConverter::" << __func__ 
                << ": Failed to allocate memory!");
        return nullptr;
    }
    for (int i = 0; i < value.size(); i++)
    {
        array[i] = value[i].toRawUTF8();
    }
    GVariant* varArray = g_variant_new_strv(array, value.size());
    delete[] array;
    return varArray;
}

/*
 * Gets the VariantType enum value that best describes a variant.
 */
GLib::VariantConverter::VariantType
GLib::VariantConverter::getType(GVariant* variant)
{
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_BOOLEAN))
    {
        return booleanType;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_BYTE))
    {
        return byteType;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_INT16))
    {
        return int16Type;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_UINT16))
    {
        return uint16Type;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_INT32)
        || g_variant_is_of_type(variant, G_VARIANT_TYPE_HANDLE))
    {
        return int32Type;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_UINT32))
    {
        return uint32Type;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_INT64))
    {
        return int64Type;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_UINT64))
    {
        return uint64Type;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_DOUBLE))
    {
        return doubleType;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING)
        || g_variant_is_of_type(variant, G_VARIANT_TYPE_OBJECT_PATH)
        || g_variant_is_of_type(variant, G_VARIANT_TYPE_SIGNATURE))
    {
        return stringType;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_DICTIONARY))
    {
        return dictType;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_BYTESTRING))
    {
        return byteStringType;
    }
    if (g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING_ARRAY))
    {
        return stringArrayType;
    }
    if (g_variant_is_container(variant))
    {
        return arrayType;
    }
    return unsupported;
}

/*
 * Gets the GType that best fits the data stored in a GVariant object. This only
 * supports a small subset of GTypes.
 */
GType GLib::VariantConverter::getGType(GVariant* variant)
{
    if (variant == nullptr)
    {
        return G_TYPE_NONE;
    }
    switch (getType(variant))
    {
        case booleanType:
            return G_TYPE_BOOLEAN;
        case byteType:
            return G_TYPE_UCHAR;
        case int16Type:
            return G_TYPE_INT;
        case uint16Type:
            return G_TYPE_UINT;
        case int32Type:
            return G_TYPE_LONG;
        case uint32Type:
            return G_TYPE_ULONG;
        case int64Type:
            return G_TYPE_INT64;
        case uint64Type:
            return G_TYPE_UINT64;
        case doubleType:
            return G_TYPE_DOUBLE;
        case stringType:
            return G_TYPE_STRING;
        case byteStringType:
            return G_TYPE_BYTE_ARRAY;
        case arrayType:
            return G_TYPE_ARRAY;
        case stringArrayType:
            return G_TYPE_STRV;
        case dictType:
        case unsupported:
            return G_TYPE_INVALID;
    }
    DBG("GLib::VariantConverter::" << __func__ 
            << ": Ignoring unhandled variant type \""
            << g_variant_get_type_string(variant) << "\"");
    return G_TYPE_INVALID;
}

/*
 * Attempts to extract a value from a GVariant object and store it within a
 * GValue object. 
 */
GValue GLib::VariantConverter::getGValue(GVariant* variant)
{
    GValue value = G_VALUE_INIT;
    g_value_init(&value, getGType(variant));
    switch (getType(variant))
    {
        case booleanType:
            g_value_set_boolean(&value, g_variant_get_boolean(variant));
            break;
        case byteType:
            g_value_set_uchar(&value, g_variant_get_byte(variant));
            break;
        case int16Type:
            g_value_set_int(&value, g_variant_get_int16(variant));
            break;
        case uint16Type:
            g_value_set_uint(&value, g_variant_get_uint16(variant));
            break;
        case int32Type:
            g_value_set_long(&value, g_variant_get_int32(variant));
            break;
        case uint32Type:
            g_value_set_ulong(&value, g_variant_get_uint32(variant));
            break;
        case int64Type:
            g_value_set_int64(&value, g_variant_get_int64(variant));
            break;
        case uint64Type:
            g_value_set_uint64(&value, g_variant_get_uint64(variant));
            break;
        case doubleType:
            g_value_set_double(&value, g_variant_get_double(variant));
            break;
        case stringType:
            g_value_set_string(&value, g_variant_get_string(variant,
                    nullptr));
            break;
        case byteStringType:
        {
            GByteArray* array = g_byte_array_new();
            gsize numBytes = 0;
            gconstpointer byteData = g_variant_get_fixed_array(variant,
                    &numBytes, sizeof(guchar));
            g_byte_array_append(array, (const guint8*) byteData, numBytes);
            g_value_take_boxed(&value, array);
            break;
        }
        case arrayType:
        {
            GArray* array = getValue<GArray*>(variant);
            if (array != nullptr)
            {
                g_value_take_boxed(&value, array);
                char* debug = g_strdup_value_contents(&value);
                DBG("Array GValue:" << debug);
                g_free(debug);
            }
            break;
        }
        case stringArrayType:
            g_value_take_boxed(&value,
                    g_variant_get_strv(variant, nullptr));
            break;
        default:
            DBG("GLib::VariantConverter::" << __func__ 
                    << ": Ignoring unhandled variant type \""
                    << g_variant_get_type_string(variant) << "\"");
    }
    return value;
}

/*
 * Converts any GVariant object into a formatted string object.
 */
juce::String GLib::VariantConverter::toString(GVariant* variant)
{
    using juce::String;
    using juce::StringArray;
    if (variant == nullptr)
    {
        return "nullptr";
    }
    switch (getType(variant))
    {
        case booleanType:
            return (g_variant_get_boolean(variant) ? "true" : "false");
        case byteType:
            return String(g_variant_get_byte(variant));
        case int16Type:
            return String(g_variant_get_int16(variant));
        case uint16Type:
            return String(g_variant_get_uint16(variant));
        case int32Type:
            return String(g_variant_get_int32(variant));
        case uint32Type:
            return String(g_variant_get_uint32(variant));
        case int64Type:
            return String(g_variant_get_int64(variant));
        case uint64Type:
            return String(g_variant_get_uint64(variant));
        case doubleType:
            return String(g_variant_get_double(variant));
        case stringType:
            return String(g_variant_get_string(variant, nullptr));
        case byteStringType:
        {
            String byteString(g_variant_get_bytestring(variant));
            if(byteString.isEmpty())
            {
                gsize stringLength = 0;
                guchar* charArray = (guchar*) g_variant_get_fixed_array
                        (variant, &stringLength, sizeof(guchar));
                for(int i = 0; i < stringLength; i++)
                {
                    byteString += (char) charArray[i];
                }
            }
            return byteString;
        }
        case arrayType:
        {
            gsize arraySize = g_variant_n_children(variant);
            if (arraySize == 0)
            {
                return "[]";
            }
            String arrayStr = "";
            iterateArray(variant, [&arrayStr, arraySize]
                    (GVariant * arrayVar)
            {
                if (arraySize == 1)
                {
                    arrayStr = toString(arrayVar);
                    return;
                }
                if (arrayStr.isEmpty())
                {
                    arrayStr = "[ ";
                }
                else
                {
                    arrayStr += ", ";
                }
                StringArray lines
                        = StringArray::fromLines(toString(arrayVar));
                lines.removeEmptyStrings();
                if (lines.isEmpty())
                {
                    return;
                }
                else if (lines.size() == 1)
                {
                    arrayStr += lines[0];
                    return;
                }
                for (const String& line : lines)
                {
                    if (arrayStr.length() > 3 
                        && !arrayStr.endsWithChar('\n'))
                    {
                        arrayStr += "\n";
                    }
                    arrayStr += "    ";
                    arrayStr += line;
                }

            });
            if (arraySize > 1)
            {
                arrayStr += "]";
            }
            return arrayStr;
        }
        case stringArrayType:
        {
            String arrayStr;
            StringArray strings = getValue<StringArray>(variant);
            for(const String& string : strings)
            {
                arrayStr += (arrayStr.isEmpty() ? "[" : ", ");
                arrayStr += string;
            }
            return arrayStr + "]";
        }
        case dictType:
        {
            String dictStr = "";
            iterateDict(variant, [&dictStr](GVariant * key, GVariant * val)
            {
                if (dictStr.isEmpty())
                {
                    dictStr = "{\n";
                }
                else
                {
                    dictStr += ",\n";
                }
                String valString = toString(val);
                if (valString.containsChar('\n'))
                {
                    valString = "\n" + valString;
                }
                String pairString = toString(key) + " = " + valString;
                StringArray lines
                        = StringArray::fromLines(pairString);
                lines.removeEmptyStrings();
                if (lines.isEmpty())
                {
                    return;
                }
                else if (lines.size() == 1)
                {
                    dictStr += "    ";
                    dictStr += lines[0];
                    return;
                }
                for (const String& line : lines)
                {
                    if (dictStr.length() > 3 && !dictStr.endsWithChar('\n'))
                    {
                        dictStr += "\n";
                    }
                    dictStr += "    ";
                    dictStr += line;
                }
            });
            if (dictStr.isEmpty())
            {
                return "{}";
            }
            return dictStr + "\n}";
        }
        default:
            return "unsupported";
    }
}

/*
 * Iterates through a GVariant* container, running a callback function for each 
 * array element.
 */ 
void GLib::VariantConverter::iterateArray(GVariant* array,
        std::function<void(GVariant*) > arrayCall)
{
    if(!g_variant_is_container(array))
    {
        DBG("GLib::VariantConverter::" << __func__ 
                << ": variant is not a container!");
        #ifdef JUCE_DEBUG
        std::cout << "\tVariant= " << toString(array) << "\n";
        #endif
        return;
    }
    GVariantIter arrayIter;
    g_variant_iter_init(&arrayIter, array);
    GVariant * child = nullptr;
    while ((child = g_variant_iter_next_value(&arrayIter)))
    {
        arrayCall(child);
        g_variant_unref(child);
        child = nullptr;
    }
}

/*
 * Iterates through a GVariant* dictionary, running a callback function for each
 * key/value pair.
 */
void GLib::VariantConverter::iterateDict(GVariant* dict,
        std::function<void(GVariant*, GVariant*) > dictCall)
{
    if(!g_variant_is_of_type(dict, G_VARIANT_TYPE_DICTIONARY))
    {
        DBG("GLib::VariantConverter::" << __func__ 
                << ": variant is not a dictionary!");
        #ifdef JUCE_DEBUG
        std::cout << "\tVariant= " << toString(dict) << "\n";
        #endif
        return;
    }
    
    GVariantIter dictIter;
    g_variant_iter_init(&dictIter, dict);
    GVariant * key = nullptr;
    GVariant * val = nullptr;
    while (g_variant_iter_next(&dictIter, "{@s*}", &key, &val))
    {
        if(g_variant_is_container(val) 
                && !g_variant_is_of_type(val, G_VARIANT_TYPE_DICTIONARY)
                && g_variant_n_children(val) == 1)
        {
            GVariant* temp = unpack(val);
            if(temp != nullptr)
            {
                g_variant_unref(val);
                val = temp;
            }
        }           
        dictCall(key, val);
        g_variant_unref(key);
        g_variant_unref(val);
        key = nullptr;
        val = nullptr;
    }
}

/*
 * Extracts all key strings from a GVariant* dictionary.
 */
juce::StringArray GLib::VariantConverter::getKeys(GVariant* dict)
{
    juce::StringArray keys;
    if (g_variant_is_of_type(dict, G_VARIANT_TYPE_DICTIONARY))
    {
        iterateDict(dict, [&keys]
                (GVariant* key, GVariant * val)
        {
            keys.add(toString(key));
        });
    }
    return keys;
}
