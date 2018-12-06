#include "GLib/SmartPointers/ErrorPtr.h"

/*
 * Creates an error pointer, optionally setting a custom error handling method.
 */
 GLib::ErrorPtr::ErrorPtr(std::function<void(GError*)> errorHandler) :
     errorHandler(errorHandler) { }

/*
 * Handles and frees the stored error structure if it is non-null.
 */ 
GLib::ErrorPtr::~ErrorPtr()
{
    handleError();
}

/*
 * Gets the address of the internal GError pointer, so that GLib functions may 
 * assign an error structure to it.
 */
GError** GLib::ErrorPtr::getAddress()
{
    return &error;
}

/*
 * If the stored error value is non-null, the error value will be passed to the 
 * errorHandler function, freed, and set to nullptr.
 */
void GLib::ErrorPtr::handleError()
{
    if(error != nullptr)
    {
        errorHandler(error);
        g_clear_error(&error);
    }
}

