#include "LibNM/NMObjects/Object.h"
#include "GLib/SmartPointers/ObjectPtr.h"

typedef GLib::ObjectPtr<NMObject*> NMObjectPtr;

/*
 * Creates a null LibNM object.
 */
LibNM::Object::Object(const GType nmType) :
GLib::Object(nmType) { }

/*
 * Creates a LibNM object sharing data with an existing LibNM object.
 */
LibNM::Object::Object(const Object& toCopy, const GType nmType) :
GLib::Object(toCopy, nmType) { }

/*
 * Creates a LibNM object to hold a NMObject pointer.
 */
LibNM::Object::Object(const NMObject* toAssign, const GType nmType) :
GLib::Object(G_OBJECT(toAssign), nmType) { }

/*
 * Gets the Object's DBus path.
 */
const char* LibNM::Object::getPath() const
{ 
    const char* path = "";
    NMObjectPtr device(NM_OBJECT(getGObject()));
    if(device != nullptr)
    {
        path = nm_object_get_path(device);
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}
