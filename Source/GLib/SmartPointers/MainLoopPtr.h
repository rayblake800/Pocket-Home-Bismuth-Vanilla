
#pragma once
#include <gio/gio.h>
#include "GLib/SmartPointers/ScopedGPointer.h"

/**
 * @file  GLib/SmartPointers/MainLoopPtr.h
 *
 * @brief  Defines a ScopedGPointer that holds a GMainLoop*. 
 */
namespace GLib
{
    /**
     * @brief  Provides the unreferencing function used for GMainLoop* data.
     */
    struct LoopUnref
    {
        /**
         * @brief  Unreferences MainLoopPtr data.
         *
         * @param context  A non-null GMainLoop pointer to unreference.
         */
        static void unref(GMainLoop* loop)
        {
            g_main_loop_unref(loop);
        }
    };

    /**
     * @brief  Stores and automatically unreferences a GMainLoop*.
     */
    typedef ScopedGPointer<GMainLoop*, LoopUnref> MainLoopPtr;
}
