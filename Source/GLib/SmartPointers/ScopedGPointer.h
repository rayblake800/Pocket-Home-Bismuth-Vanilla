#pragma once
#include <glib-object.h>
#include "GLib/GLib.h"

/**
 * @file  ScopedObjectPtr.h
 *
 * @brief  A GObject pointer that automatically unreferences any held non-null
 *         value it holds on destruction.
 */
template <typename GPointerType>
class GLib::ScopedGPointer
{
public:
    ScopedGPointer(GPointerType source) : data(source) { }

    operator GPointerType() const
    {
        return data;
    }
    
    virtual ~ScopedGPointer()
    {
        if(data != nullptr)
        {
            unref();
        }
    }

private:
    virtual void unref() const
    {
        g_object_unref(data);
    }

    GPointerType data;
};
