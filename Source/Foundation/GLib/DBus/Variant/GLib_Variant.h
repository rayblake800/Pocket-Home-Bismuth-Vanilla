#pragma once
/**
 * @file  GLib/DBus/Variant/Variant.h
 *
 * @brief  An incomplete container object for GLib's GVariant objects.
 */

namespace GLib { class Variant; }

class GLib::Variant
{
    enum class GVarType
    {
        boolean,
        byte,
        int16,
        uint16,
        int32,
        uint32,
        int64,
        uint64,
        handle,
        doubleFloat,
        string,
        objectPath,
        signature,
        variant,
        array,
        tuple,
        emptyTuple,
        dictEntry,
        dictionary,
        stringArray,
        objectPathArray,
        byteString,
        byteStringArray,
        varDict
    };

    // template<typename CheckedType>
    // bool isType()
    //
    // Type getType()
    //
    // template<typename StoredType>
    // StoredType getValue(StoredType defaultValue);
    // 

};
