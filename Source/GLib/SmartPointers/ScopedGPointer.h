#pragma once
#include <glib-object.h>
#include "GLib/GLib.h"

/**
 * @file  ScopedObjectPtr.h
 *
 * @brief  An immutable GObject pointer that automatically unreferences any 
 *         non-null value it holds on destruction.
 *
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
 * ensure it is properly unreferenced when it goes out of scope. The
 * ScopedGPointer's Unreferencer template parameter may be used to change the
 * method of dereferencing used, since not all GLib data uses the g_object_unref
 * method.
 *
 *   When GLib GObject data needs to be copied, shared, replaced, or connected 
 * to signal handlers, define a GLib::Object subclass instead of using a 
 * ScopedGPointer. Always make sure GLib data will actually need to be
 * unreferenced before using it with a ScopedGPointer.
 */

/**
 * @brief  Provides the unreferencing function, g_object_unref(), that 
 *         ScopedGPointer calls on its data pointer on destruction. 
 *
 * @tparam GPointerType  The type of GLib pointer being unreferenced. When using
 *                       the default unreferencer, this should always be
 *                       GObject*, or some GObject subclass pointer that doesn't
 *                       require its own unreferencing function.
 */
template <typename GPointerType>
struct DefaultUnref
{
    /**
     * @brief  Unreferences a ScopedGPointer's data pointer using the 
     *         g_object_unref() method from the GLib library.
     *
     * @param dataPtr  A GLib data pointer that needs to be unreferenced.
     */
    static void unref(GPointerType dataPtr)
    {
        g_object_unref(G_OBJECT(dataPtr));
    }
};

/**
 * @brief  Holds and provides access to a GLib data pointer.
 *
 * @tparam GPointerType  The specific data pointer type being held.
 *
 * @tparam Unreferencer  A struct defining the static 
 *                       unref(GPointerType dataPtr) method used to unreference
 *                       or free the data pointer on destruction.
 */
template <typename GPointerType, typename Unreferencer
        = DefaultUnref<GPointerType>>
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
