#pragma once
/**
 * @file  GLib_LoopPtr.h
 *
 * @brief  Provides a ScopedGPointer class that holds GMainLoop data pointers.
 */

#include "GLib_ScopedGPointer.h"
#include <gio/gio.h>

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
    typedef ScopedGPointer<GMainLoop*, LoopUnref> LoopPtr;
}
