#pragma once
/**
 * @file  GLib_ErrorPtr.h
 *
 * @brief  Defines a smart pointer that holds a GError*. 
 */

#include "GLib_ScopedGPointer.h"
#include "JuceHeader.h"
#include <gio/gio.h>

/**
 *  In typical use, the address of a null GError* is passed to a function, which
 * may or may not create a GError and store it in the GError*. After the
 * function call, any assigned GError* needs to be checked, handled, and freed.
 *
 *  ErrorPtr's getAddress() method may be passed in to any method that takes a 
 * GError**. When the ErrorPtr goes out of scope or its handleError() method is
 * called, if its error value is non-null, it will handle and clear the error.
 * Error handling uses either a custom error handling method passed in on the
 * ErrorPtr's construction, or a default handler that simply prints a debug
 * message.
 */
namespace GLib
{
    class ErrorPtr
    {
    public:
        /**
         * @brief  Creates an error pointer, optionally setting a custom error
         *         handling method.
         *
         * @param errorHandler  A function to run if the errorHandler's error
         *                      object is set.  If omitted, errors will just be
         *                      printed as debug output.
         */
        ErrorPtr(std::function<void(GError*)> errorHandler = [](GError* error)
                { 
                    DBG("Error: " << juce::String(error->message)); 
                });

        /**
         * @brief  Creates an error pointer to manage an existing error value.
         *
         * @param error  A GError value that will be freed (if non-null) when
         *               the ErrorPtr is destroyed.
         */
        ErrorPtr(GError* error);

        /**
         * @brief  Handles and frees the stored error structure if it is 
         *         non-null.
         */
        virtual ~ErrorPtr();

        /**
         * @brief  Gets the address of the internal GError pointer, so that GLib
         *         functions may assign an error structure to it.
         *
         * @return  The error pointer's address. 
         */
        GError** getAddress();

        /**
         * @brief  If the stored error value is non-null, the error value will
         *         be passed to the errorHandler function, freed, and set to
         *         nullptr.
         */
        void handleError();
    
    private:
        /* The function used to handle non-null error values. */
        std::function<void(GError*)> errorHandler;

        /* The internal GError pointer. */
        GError* error = nullptr;
    };
}
