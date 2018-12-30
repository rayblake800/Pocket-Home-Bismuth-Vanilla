#include "LibNM/OwnedObjects/OwnedObject.h"
#include "LibNM/ContextTest.h"
#include "GLib/SmartPointers/ObjectPtr.h"

typedef GLib::ObjectPtr<NMObject*> NMObjectPtr;

/*
 * Creates a null LibNM object.
 */
LibNM::OwnedObject::OwnedObject(const GType nmType) : GLib::Object(nmType) { }

/*
 * Creates a LibNM object sharing data with an existing LibNM object.
 */
LibNM::OwnedObject::OwnedObject(const OwnedObject& toCopy, const GType nmType) :
GLib::Object(toCopy, nmType) 
{ 
    ASSERT_NM_CONTEXT;
}

/*
 * Creates a LibNM object to hold a NMObject pointer.
 */
LibNM::OwnedObject::OwnedObject(const NMObject* toAssign, const GType nmType) :
GLib::Object(G_OBJECT(toAssign), nmType)
{ 
    ASSERT_NM_CONTEXT;
}

/*
 * Gets the Object's DBus path.
 */
const char* LibNM::OwnedObject::getPath() const
{ 
    ASSERT_NM_CONTEXT;
    const char* path = "";
    NMObjectPtr object(NM_OBJECT(getGObject()));
    if(object != nullptr)
    {
        path = nm_object_get_path(object);
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}
