#pragma once
#include "GLib_VariantTypes.h"

namespace GLib
{
    template<typename T>
    struct TypeHelper
    {
        /**
         * @brief  Gets the GVariantType of the TypeHelper's type.
         *
         * @return  The GVariantType* of type T. 
         */
        static const GVariantType* getGVariantType() 
        {
            DBG("Invalid variant type!");
            jassertfalse;
            return NULL;
        }

        /**
         * @brief  Gets a string describing the TypeHelper's type.
         *
         * @return  The name of type T. 
         */
        static VariantTypes::String getTypeName()
        {
            DBG("Invalid variant type!");
            jassertfalse;
            return "Invalid";
        }

        /**
         * @brief  Gets the TypeHelper's GLib format string.
         *
         * @return  The format string for type T. 
         */
        static VariantTypes::String getFormatString() 
        {
            DBG("Invalid variant type!");
            jassertfalse;
            return "";
        }

        /**
         * @brief  Checks if a GVariant is holding this TypeHelper's type.
         *
         * @param variant  A non-null GVariant pointer.
         *
         * @return  Whether the variant holds data of type T. 
         */
        static bool isType(GVariant* variant)
        {
            return g_variant_is_of_type(variant, 
                    TypeHelper<T>::getGVariantType());
        }

        /**
         * @brief  Gets the value stored in a GVariant.
         *
         * @param variant       Any GVariant pointer.
         *
         * @param defaultValue  A default value to return if the variant is
         *                      invalid, or not of type T.
         *
         * @return              The variant's stored value if possible, or
         *                      the default value if not.
         */
        static T getValue(GVariant* variant, T defaultValue)
        {
            DBG("Invalid variant type!");
            jassertfalse;
            return defaultValue;
        }

        /**
         * @brief  Creates a GVariant holding a value of this TypeHelper's
         *         type.
         *
         * @param value  The value to store in the new variant.
         *
         * @return       A new GVariant* holding the given value.
         */
        static GVariant* getVariant(T value)
        {
            DBG("Invalid variant type!");
            jassertfalse;
            return nullptr;
        }
    };

    // Array types:
#include "GLib_TypeHelper_Array.inc"

    /**
     * @brief  Declares all TypeHelper template specializations for a supported
     *         GVariant value type.
     *
     * @param VarType  A supported GVariant data type.
     */
    #define TYPE_DECLARATION(VarType) \
    template<> juce::String TypeHelper<VarType>::getTypeName();            \
    template<> juce::String TypeHelper<VarType>::getFormatString();        \
    template<> bool TypeHelper<VarType>::isType(GVariant* variant);        \
    template<> const GVariantType* TypeHelper<VarType>::getGVariantType(); \
    template<> VarType TypeHelper<VarType>::getValue(GVariant*, VarType);  \
    template<> GVariant* TypeHelper<VarType>::getVariant(VarType);

    TYPE_DECLARATION(bool);
    TYPE_DECLARATION(VariantTypes::byte);
    TYPE_DECLARATION(VariantTypes::int16);
    TYPE_DECLARATION(VariantTypes::uint16);
    TYPE_DECLARATION(VariantTypes::int32);
    TYPE_DECLARATION(VariantTypes::uint32);
    TYPE_DECLARATION(VariantTypes::int64);
    TYPE_DECLARATION(VariantTypes::uint64);
    TYPE_DECLARATION(VariantTypes::Handle);
    TYPE_DECLARATION(double);
    TYPE_DECLARATION(VariantTypes::String);
    TYPE_DECLARATION(VariantTypes::Path);
    TYPE_DECLARATION(VariantTypes::Signature);
    TYPE_DECLARATION(VariantTypes::GVariantType);
}
