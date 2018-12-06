#pragma once
#include "JuceHeader.h"

/**
 * @file  DesktopEntry/Implementation.h
 *
 * @brief  Defines the private classes used to implement the DesktopEntry
 *         module.
 *
 * The classes defined within this file may not be used outside of the 
 * DesktopEntry implementation.
 *
 * @see  DesktopEntry/DesktopEntry.h
 */
namespace DesktopEntry
{

    /**
     * @brief  The private SharedResource class that loads, caches, and updates
     *         desktop entry file data.
     *
     * @see  DesktopEntry/LoadingThread.h
     */
    class LoadingThread;
    
    /**
     * @brief  The interface used by the LoadingThread to send data to 
     *         UpdateListener objects.
     *
     * @see  DesktopEntry/UpdateListener.h
     */
    class UpdateInterface;

    /**
     * @brief  Defines a set of functions used by the EntryFile to process
     *         desktop entry data.
     *
     * @see  DesktopEntry/FileUtils.h
     */
    namespace FileUtils {}
}
