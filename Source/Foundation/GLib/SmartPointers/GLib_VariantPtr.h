#pragma once
/**
 * @file  GLib_VariantPtr.h
 *
 * @brief  Provides a ScopedGPointer class that holds GVariant data pointers. 
 */

#include "GLib_ScopedGPointer.h"
#include <gio/gio.h>

namespace GLib
{
    /**
     * @brief  Provides the unreferencing function used for GVariant* data.
     */
    struct VariantUnref
    {
        /**
         * @brief  Unreferences GVariant data.
         *
         * @param context  A non-null variant pointer to unreference.
         */
        static void unref(GVariant* variant)
        {
            g_variant_unref(variant);
        }
    };

    /**
     * @brief  Stores and automatically unreferences a GVariant*.
     */
    typedef ScopedGPointer<GVariant*, VariantUnref> VariantPtr;
}
