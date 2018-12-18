#pragma once
/**
 * @file GLib/Variant/VariantTypes.h
 *
 * @brief  Defines all types that may be stored in GLib::Variant objects.
 */

#include "GLib/DBus/Variant/BasicValueWrapper.h"
#include "JuceHeader.h"
#include <gio/gio.h>
#include <tuple>
#include <map>

namespace GLib
{
    class Variant;

    namespace VariantTypes
    {
        // All valid Variant data types:

        // no typedef/declaration needed: bool, double, Variant

        /* Integer data types: */
        typedef juce::uint8  byte;
        typedef juce::int16  int16;
        typedef juce::uint16 uint16;
        typedef juce::int32  int32;
        typedef juce::uint32 uint32;
        typedef juce::int64  int64;
        typedef juce::uint64 uint64;

        /* Used as an index into an array of file descriptors sent alongside a 
           D-Bus message: */
        struct Handle : public BasicValueWrapper<uint32>
        {
            using BasicValueWrapper::BasicValueWrapper;
        };

        typedef juce::String String;

        /* Used to identify DBus objects: */
        class Path : public BasicValueWrapper<String>
        {
            using BasicValueWrapper::BasicValueWrapper;
        };

        /* Strings used for DBus type signatures: */
        class Signature : public BasicValueWrapper<String>
        {
            using BasicValueWrapper::BasicValueWrapper;
        };

        typedef GVariant* GVariantType;

        /* All array types: */
        template<class T>
        using Array = juce::Array<T>;

        /* All tuple types: */
        template< class... Types >
        using Tuple = std::tuple< Types ... >;

        /* The empty tuple type: */
        typedef std::tuple<> Unit;

        /* All key/value pair types: */
        template<class KeyType, class ValueType>
        using DictEntry = std::pair<KeyType, ValueType>;

        /* All dictionary types: */
        template<class KeyType, class ValueType>
        using Dictionary = std::map<KeyType, ValueType>;

        typedef juce::StringArray StringArray;

        /* An array of object path strings: */
        class ObjectPathArray : public BasicValueWrapper<StringArray>
        {
            using BasicValueWrapper::BasicValueWrapper;
        };

        typedef juce::Array<byte>          ByteString;

        typedef juce::Array<ByteString>    ByteStringArray;

        typedef Dictionary<String,Variant> VarDict;
    }
}
