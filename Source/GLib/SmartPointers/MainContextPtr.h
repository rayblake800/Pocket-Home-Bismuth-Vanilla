#pragma once
#include <gio/gio.h>
#include "GLib/SmartPointers/ScopedGPointer.h"

/**
 * @file  GLib/SmartPointers/MainContextPtr.h
 *
 * @brief  Defines a ScopedGPointer that holds a GMainContext*. 
 */
namespace GLib
{
    /**
     * @brief  Provides the unreferencing function used for GMainContext* data.
     */
    struct ContextUnref
    {
        /**
         * @brief  Unreferences MainContextPtr data.
         *
         * @param context  A non-null context pointer to unreference.
         */
        static void unref(GMainContext* context)
        {
            g_main_context_unref(context);
        }
    };

    /**
     * @brief  Stores and automatically unreferences a GMainContext*.
     */
    typedef ScopedGPointer<GMainContext*, ContextUnref> MainContextPtr;
}
