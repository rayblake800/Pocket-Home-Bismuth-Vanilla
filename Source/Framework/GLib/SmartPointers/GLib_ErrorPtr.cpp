#include "GLib_ErrorPtr.h"

// Creates an error pointer, optionally setting a custom error handling
// function.
GLib::ErrorPtr::ErrorPtr(std::function<void(GError*)> errorHandler) :
    errorHandler(errorHandler) { }


// Creates an error pointer to manage an existing error value.
GLib::ErrorPtr::ErrorPtr(GError* error) : error(error),
errorHandler([](GError* error)
{
    DBG("Error: " << juce::String(error->message));
}) { }


// Handles and frees the stored error structure if it is non-null.
GLib::ErrorPtr::~ErrorPtr()
{
    handleError();
}


// Gets the address of the internal GError pointer, so that GLib functions may
// assign an error structure to it.
GError** GLib::ErrorPtr::getAddress()
{
    return &error;
}


// If the stored error value is non-null, the error value will be passed to the
// errorHandler function, freed, and set to nullptr.
void GLib::ErrorPtr::handleError()
{
    if (error != nullptr)
    {
        if (errorHandler)
        {
            errorHandler(error);
        }
        g_clear_error(&error);
    }
}

