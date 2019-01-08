#pragma once
/**
 * @file  GLib_BasicValueWrapper.h
 *
 * @brief  Wraps a basic value type in an object wrapper, which can be treated 
 *         exactly like the contained type.
 */

namespace GLib { template <typename basicType> class BasicValueWrapper; }

/**
 * Use this to allow different template specializations for synonymous types.
 *
 * @tparam basicType  The basic data type this wrapper contains.
 */
template <typename basicType>
class GLib::BasicValueWrapper
{
public:
    BasicValueWrapper(basicType value) : value(value) { }
    virtual ~BasicValueWrapper() { }

    operator basicType&()
    {
        return value;
    };

private:
    basicType value;
};
