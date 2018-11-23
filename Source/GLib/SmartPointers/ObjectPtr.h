#pragma once
#include <glib-object.h>
#include "GLib/SmartPointers/ScopedGPointer.h"

/**
 * @file  GLib/SmartPointers/MainContextPtr.h
 *
 * @brief  Defines a ScopedGPointer that holds a generic GObject, or optionally
 *         a specific GObject type.
 */
namespace GLib
{
    /**
     * @brief  Provides the unreferencing function used for GMainContext* data.
     *
     * @tparam GObjectType  The type of GObject* held by the pointer.
     */
    template<typename GObjectType>
    struct ObjectUnref
    {
        /**
         * @brief  Unreferences GObject data.
         *
         * @param context  A non-null GObject pointer to unreference.
         */
        static void unref(GObjectType object)
        {
            g_object_unref(G_OBJECT(object));
        }
    };

    /**
     * @brief  Stores and automatically unreferences a GMainContext*.
     *
     * @tparam GObjectType  The type of GObject held by the pointer.
     */
    template<typename GObjectType = GObject*>
    class ObjectPtr : 
        public ScopedGPointer<GObjectType, ObjectUnref<GObjectType>>
    {
    public:
        using ScopedGPointer<GObjectType,ObjectUnref<GObjectType>>
            ::ScopedGPointer;
    };
}
