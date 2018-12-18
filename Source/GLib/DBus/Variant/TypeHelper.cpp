#include "GLib/DBus/Variant/TypeHelper.h"

/* Use templates to define simple TypeHelper specialization methods and avoid
 * excess repetition: */

#define TYPE_DEFINITION(VarType, GVarType)                 \
template<>                                                 \
const GVariantType* TypeHelper<VarType>::getGVariantType() \
{                                                          \
    return GVarType;                                       \
}                                                          \
                                                           \
template<>                                                 \
bool TypeHelper<VarType>::isType(GVariant* variant)        \
{                                                          \
    return g_variant_is_of_type(variant,                   \
            TypeHelper<VarType>::getGVariantType());       \
}

#define TYPE_NAME_DEFINITION(VarType, typeName) \
template<>                                      \
juce::String TypeHelper<VarType>::getTypeName() \
{                                               \
    return typeName;                            \
}

#define FORMAT_STRING_DEFINITION(VarType, formatString) \
template<>                                              \
juce::String TypeHelper<VarType>::getFormatString()     \
{                                                       \
    return formatString;                                \
}

#define TYPE_CHECK_DEFINITION(VarType, checkFunction) \
template<>                                            \
bool TypeHelper<VarType>::isType(GVariant* variant)   \
{                                                     \
    return checkFunction(variant);                    \
}

#define GET_VALUE_DEFINITION(VarType, getFunction)       \
template<>                                               \
VarType TypeHelper<VarType>::getValue                    \
(GVariant* variant, VarType defaultValue)                \
{                                                        \
    if(variant == nullptr                                \
            || !TypeHelper<VarType>::isType(variant))    \
    {                                                    \
        return defaultValue;                             \
    }                                                    \
    return VarType(getFunction(variant));                \
}

#define GET_VARIANT_DEFINITION(VarType, varFunction)     \
template<>                                               \
GVariant* TypeHelper<VarType>::getVariant(VarType value) \
{                                                        \
    return varFunction(value);                           \
}

#define BASIC_TYPE_DEFINITION(type, G_TYPE, name, formatStr, getter, setter)  \
TYPE_DEFINITION(type, G_TYPE);                                                \
TYPE_NAME_DEFINITION(type, name);                                             \
FORMAT_STRING_DEFINITION(type, formatStr);                                    \
GET_VALUE_DEFINITION(type, getter);                                           \
GET_VARIANT_DEFINITION(type, setter);                                         \

namespace GLib
{
    // boolean type:
    BASIC_TYPE_DEFINITION(bool, 
            G_VARIANT_TYPE_BOOLEAN,
            "boolean",
            "b",
            g_variant_get_boolean,
            g_variant_new_boolean);

    // byte type:
    BASIC_TYPE_DEFINITION(VariantTypes::byte, 
            G_VARIANT_TYPE_BYTE,
            "byte",
            "y",
            g_variant_get_byte,
            g_variant_new_byte);

    // int16 type:
    BASIC_TYPE_DEFINITION(VariantTypes::int16, 
            G_VARIANT_TYPE_INT16,
            "int16",
            "n",
            g_variant_get_int16,
            g_variant_new_int16);

    // uint16 type:
    BASIC_TYPE_DEFINITION(VariantTypes::uint16, 
            G_VARIANT_TYPE_UINT16,
            "uint16",
            "q",
            g_variant_get_uint16,
            g_variant_new_uint16);

    // int32 type:
    BASIC_TYPE_DEFINITION(VariantTypes::int32, 
            G_VARIANT_TYPE_INT32,
            "int32",
            "i",
            g_variant_get_int32,
            g_variant_new_int32);

    // uint32 type:
    BASIC_TYPE_DEFINITION(VariantTypes::uint32, 
            G_VARIANT_TYPE_UINT32,
            "uint32",
            "u",
            g_variant_get_uint32,
            g_variant_new_uint32);

    // int64 type:
    BASIC_TYPE_DEFINITION(VariantTypes::int64, 
            G_VARIANT_TYPE_INT64,
            "int64",
            "x",
            g_variant_get_int64,
            g_variant_new_int64);

    // uint64 type:
    BASIC_TYPE_DEFINITION(VariantTypes::uint64, 
            G_VARIANT_TYPE_UINT64,
            "uint64",
            "t",
            g_variant_get_uint64,
            g_variant_new_uint64);

    // handle type:
    BASIC_TYPE_DEFINITION(VariantTypes::Handle, 
            G_VARIANT_TYPE_HANDLE,
            "handle",
            "h",
            g_variant_get_handle,
            g_variant_new_handle);

    // double type:
    BASIC_TYPE_DEFINITION(double, 
            G_VARIANT_TYPE_DOUBLE,
            "double",
            "d",
            g_variant_get_double,
            g_variant_new_double);

    static const char* getVarString(GVariant* var)
    {
        return g_variant_get_string(var, NULL);
    }

    static GVariant* newVarString(VariantTypes::String toVar)
    {
        return g_variant_new_string(toVar.toRawUTF8());
    }

    // String type:
    BASIC_TYPE_DEFINITION(VariantTypes::String, 
            G_VARIANT_TYPE_STRING,
            "String",
            "s",
            getVarString,
            newVarString);

    // Path type:
    BASIC_TYPE_DEFINITION(VariantTypes::Path, 
            G_VARIANT_TYPE_OBJECT_PATH,
            "Path",
            "o",
            getVarString,
            newVarString);
    
    // Signature type:
    BASIC_TYPE_DEFINITION(VariantTypes::Signature, 
            G_VARIANT_TYPE_SIGNATURE,
            "Signature",
            "g",
            getVarString,
            newVarString);

    // Variant type:
    BASIC_TYPE_DEFINITION(VariantTypes::GVariantType, 
            G_VARIANT_TYPE_VARIANT,
            "GVariant*",
            "v",
            g_variant_get_variant,
            g_variant_new_variant);
    // Maybe types:

    // Tuple types:
    // Empty tuple type:
    // Dictionary entry types:
    // Dictionary types:
    // StringArray type:
    // ObjectPathArray type:
    // ByteString type:
    // ByteStringArray type:
    // VarDict type:
}
