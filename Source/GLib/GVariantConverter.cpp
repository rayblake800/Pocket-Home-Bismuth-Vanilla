#include "GVariantConverter.h"
#include "JuceHeader.h"

namespace GVariantConverter
{

    GVariant* unpack(GVariant* container)
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

    template<> bool getValue(GVariant* variant)
    {
        if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_BOOLEAN))
        {
            jassertfalse;
            return false;
        }
        return g_variant_get_boolean(variant);
    }

    template<> guint32 getValue(GVariant* variant)
    {
        if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_UINT32))
        {
            jassertfalse;
            return 0;
        }
        return g_variant_get_int32(variant);
    }

    template<> String getValue(GVariant* variant)
    {
        if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING))
        {
            jassertfalse;
            return "";
        }
        return g_variant_get_string(variant, nullptr);
    }

    template<> StringArray getValue(GVariant* variant)
    {
        if (!g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING_ARRAY))
        {
            jassertfalse;
            return {};
        }
        gsize arraySize = 0;
        const gchar ** array = g_variant_get_strv(variant, &arraySize);
        StringArray varArray;
        for (int i = 0; i < arraySize; i++)
        {
            varArray.add(String(array[i]));
        }
        free(array);
        return varArray;
    }

    template<> GArray* getValue(GVariant* variant)
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
    
    template<> GByteArray* getValue(GVariant* variant)
    {
        GByteArray* array = g_byte_array_new();
        gsize numBytes = 0;
        gconstpointer byteData = g_variant_get_fixed_array(variant,
                &numBytes, sizeof(guchar));
        g_byte_array_append(array, (const guint8*) byteData, numBytes);
        return array;
    }

    template<> GVariant* getVariant(bool value)
    {
        return g_variant_new_boolean(value);
    }

    template<> GVariant* getVariant(guint32 value)
    {
        return g_variant_new_uint32(value);
    }

    template<> GVariant* getVariant(String value)
    {
        return g_variant_new_string(value.toRawUTF8());
    }

    template<> GVariant* getVariant(StringArray value)
    {
        const char ** array = new (std::nothrow) const char*[value.size()];
        if (array == nullptr)
        {
            DBG(__func__ << ":Failed to allocate memory!");
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

    GVariantConverter::VariantType getType(GVariant* variant)
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
        if (g_variant_is_container(variant))
        {
            return arrayType;
        }
        return unsupported;
    }

    GType getGType(GVariant* variant)
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
            case dictType:
            case unsupported:
                return G_TYPE_INVALID;
        }
    }

    GValue getGValue(GVariant* variant)
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
                g_value_set_string(&value, g_variant_get_string(variant, nullptr));
                break;
            case byteStringType:
            {
                GByteArray* array = g_byte_array_new();
                gsize numBytes = 0;
                gconstpointer byteData = g_variant_get_fixed_array(variant,
                        &numBytes, sizeof(guchar));
                g_byte_array_append(array, (const guint8*) byteData, numBytes);
                g_value_take_boxed(&value, array);
                char* debug = g_strdup_value_contents(&value);
                DBG("Byte array GValue:" << debug);
                g_free(debug);
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
        }
        return value;
    }

    String toString(GVariant* variant)
    {
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
                return String(g_variant_get_bytestring(variant));
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
                        if (arrayStr.length() > 3 && !arrayStr.endsWithChar('\n'))
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
            case unsupported:
                return "unsupported";
        }
    }

    void iterateArray(GVariant* array,
            std::function<void(GVariant*) > arrayCall)
    {
        g_assert(g_variant_is_container(array));
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

    void iterateDict(GVariant* dict,
            std::function<void(GVariant*, GVariant*) > dictCall)
    {
        g_assert(g_variant_is_of_type(dict, G_VARIANT_TYPE_DICTIONARY));
        GVariantIter dictIter;
        g_variant_iter_init(&dictIter, dict);
        GVariant * key = nullptr;
        GVariant * val = nullptr;
        while (g_variant_iter_next(&dictIter, "{@s*}", &key, &val))
        {

            if(g_variant_is_container(val) 
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

    StringArray getKeys(GVariant* dict)
    {
        StringArray keys;
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
};

