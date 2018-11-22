#pragma once
#include <glib-object.h>
#include "GLib/GLib.h"

/**
 * @file  ScopedObjectPtr.h
 *
 * @brief  An immutable GObject pointer that automatically unreferences any 
 *         non-null value it holds on destruction.
 */
template <typename GPointerType>
class GLib::ScopedGPointer
{
public:
    /**
     * @brief  Create the ScopedGPointer, permanently setting its internal
     *         data pointer.
     *
     * @param source  The data pointer this ScopedGPointer will hold.
     */
    ScopedGPointer(GPointerType source) : data(source) { }

    /**
     * @brief  Allows the ScopedGPointer to be automatically cast to its basic
     *         type, so it can be directly passed to library functions.
     *
     * @return  The ScopedGPointer's held data pointer. 
     */
    operator GPointerType() const
    {
        return data;
    }
    
    /**
     * @brief  Unreferences the data pointer on destruction, unless it is null. 
     */
    virtual ~ScopedGPointer()
    {
        if(data != nullptr)
        {
            unref();
        }
    }

private:
    /**
     * @brief  Defines how the ScopedGPointer unreferences its data on 
     *         destruction.  This calls g_object_unref() by default, override
     *         for GPointerTypes with unique unreference methods. 
     */
    virtual void unref() const
    {
        g_object_unref(data);
    }

    /* Holds the ScopedGPointer's raw data pointer. */
    GPointerType data;
};
