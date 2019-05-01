#include "GLib_Borrowed_SharedContainer.h"

// Creates a SharedContainer holding GObject* data.
GLib::Borrowed::SharedContainer::SharedContainer(GObject* object) :
Nullable<GObject*>(object) { }


// Casts the SharedContainer directly to its stored GObject* value.
GLib::Borrowed::SharedContainer::operator GObject*() const
{
    return getData();
}


// Permanently sets the SharedContainer object's data to nullptr.
void GLib::Borrowed::SharedContainer::clearData()
{
    getDataReference() = nullptr;
}
