#include "GVariantConverter.h"
#include "JuceHeader.h"

namespace GVariantConverter
{

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
        if (g_variant_is_container(variant))
        {
            return arrayType;
        }
        return unsupported;
    }

    String toString(GVariant* variant)
    {
        if(variant == nullptr)
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
                return String(g_variant_get_string(variant,nullptr));
            case arrayType:
            {
                gsize arraySize = g_variant_n_children(variant);
                if(arraySize == 0)
                {
                    return "[]";
                }
                String arrayStr = "";
                iterateArray(variant, [&arrayStr, arraySize]
                (GVariant * arrayVar)
                {
                    if(arraySize == 1)
                    {
                        arrayStr = toString(arrayVar);
                        return;
                    }
                    if(arrayStr.isEmpty())
                    {
                        arrayStr = "[\n";
                    }
                    else
                    {
                        arrayStr += ",\n";
                    }
                    StringArray lines
                            = StringArray::fromLines(toString(arrayVar));
                    for(const String& line : lines)
                    {
                        arrayStr += "    ";
                        arrayStr += line;
                        arrayStr += "\n";
                    }
                    
                });
                if(arraySize > 1)
                {
                    arrayStr += "\n]";
                }
                return arrayStr;
            }
            case dictType:
            {
                String dictStr = "";
                iterateDict(variant, [&dictStr](GVariant * key, GVariant* val)
                {
                    if(dictStr.isEmpty())
                    {
                        dictStr = "{\n";
                    }
                    else
                    {
                        dictStr += ",\n";
                    }
                    String valString = toString(val);
                    if(valString.containsChar('\n'))
                    {
                        valString = "\n" + valString;
                    }
                    String pairString = toString(key) + " = " + valString;
                    StringArray lines
                            = StringArray::fromLines(pairString);
                    lines.removeEmptyStrings();
                    if(lines.isEmpty())
                    {
                        return;
                    }
                    else if(lines.size() == 1)
                    {
                        dictStr += "    ";
                        dictStr += lines[0];
                        return;
                    }
                    for(const String& line : lines)
                    {
                        if(dictStr.length() > 3 && !dictStr.endsWithChar('\n'))
                        {
                            dictStr += "\n";
                        }
                        dictStr += "    ";
                        dictStr += line;
                    }
                    
                });
                if(dictStr.isEmpty())
                {
                    return "{}";
                }
                return dictStr + "\n}";
            }
            case unsupported:
                return "unsupported";
        }
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
            std::function<void(GVariant*, GVariant*)> dictCall)
    {
        g_assert(g_variant_is_of_type(dict, G_VARIANT_TYPE_DICTIONARY));
        GVariantIter dictIter;
        g_variant_iter_init(&dictIter, dict);
        GVariant * key = nullptr;
        GVariant * val = nullptr;
        while (g_variant_iter_next(&dictIter, "{@s*}", &key, &val))
        {
            dictCall(key, val);
            g_variant_unref(key);
            g_variant_unref(val);
            key = nullptr;
            val = nullptr;
        }
    }
};

