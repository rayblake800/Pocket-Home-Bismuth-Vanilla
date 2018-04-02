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
        if (g_variant_is_of_type(variant, G_VARIANT_TYPE_UINT32))
        {
            return uint32Type;
        }
        if (g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING))
        {
            return stringType;
        }
        if (g_variant_is_of_type(variant, G_VARIANT_TYPE_STRING_ARRAY))
        {
            return stringArrayType;
        }
        return unsupported;
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
};