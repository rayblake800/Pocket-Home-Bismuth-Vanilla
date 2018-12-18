#pragma once
/**
 * @file  ScopedObjectPtr.h
 *
 * @brief  An immutable GObject pointer that automatically unreferences any 
 *         non-null value it holds on destruction.
 */

#include <glib-object.h>

namespace GLib { template <typename GPointerType, typename Unreferencer>
    class ScopedGPointer; }

/**
 *  The GLib library introduces many useful reference counted data structures 
 * and pseudo-objects. However, as GLib is written entirely in C, these 
 * structures must be explicitly unreferenced. ScopedObjectPtrs serve as 
 * wrappers for these GLib pointers, automatically unreferencing them on 
 * destruction.
 *
 *  Many GLib functions return dynamically allocated data with the expectation 
 * that the caller will free or unreference that data once it is no longer
 * needed. When that data is only needed within a single scope and does not
 * need to be copied elsewhere, it should be wrapped in a ScopedGPointer to 
 * ensure it is properly unreferenced when it goes out of scope. The specific
 * unreferencing function needed by the pointer is set by its Unreferencer
 * template parameter, which should call the correct GLib unreference or free
 * function within a static unref method.
 *
 *   When GLib GObject data needs to be copied, shared, replaced, or connected 
 * to signal handlers, define a GLib::Object subclass instead of using a 
 * ScopedGPointer. Always make sure GLib data will actually need to be
 * unreferenced before using it with a ScopedGPointer.
 *
 * @tparam GPointerType  The specific data pointer type being held.
 *
 * @tparam Unreferencer  A struct defining the static 
 *                       unref(GPointerType dataPtr) method used to unreference
 *                       or free the data pointer on destruction.
 */
template <typename GPointerType, typename Unreferencer>
class GLib::ScopedGPointer
{
public:
    /**
     * @brief  Create the ScopedGPointer, permanently setting its internal
     *         data pointer.
     *
     * @param dataPtr  The data pointer this ScopedGPointer will hold.
     */
    ScopedGPointer(GPointerType dataPtr) : dataPtr(dataPtr) { }

    /**
     * @brief  Allows the ScopedGPointer to be automatically cast to its basic
     *         type, so it can be directly passed to library functions.
     *
     * @return  The ScopedGPointer's held data pointer. 
     */
    operator GPointerType() const
    {
        return dataPtr;
    }
    
    /**
     * @brief  Unreferences the data pointer on destruction, unless it is null. 
     */
    virtual ~ScopedGPointer()
    {
        if(dataPtr != nullptr)
        {
            Unreferencer::unref(dataPtr);
        }
    }

private:
    /* Holds the ScopedGPointer's raw data pointer. */
    GPointerType dataPtr;
};
