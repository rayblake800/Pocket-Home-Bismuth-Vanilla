#include "GLib/SmartPointers/ObjectPtr.h"
#include "GLib/Object.h"

/*
 * Create this ObjectPtr to access a GLib::Object's internal GObject* data.
 */
GLib::ObjectPtr::ObjectPtr(const Object& dataSource) : 
    objectData(dataSource.getGObject()), 
    shouldUnreference(dataSource.isOwned()) { }

/*
 * Create an ObjectPtr to hold a GObject* value.
 */
GLib::ObjectPtr::ObjectPtr(GObject* dataSource) : 
    objectData(dataSource), shouldUnreference(true) { }


/*
 * If necessary, unreferences the stored GObject* value.
 */
GLib::ObjectPtr::~ObjectPtr()
{
    if(objectData != nullptr && shouldUnreference)
    {
        g_object_unref(objectData);
    }
}

/*
 * Allows this ObjectPtr to be directly used as if it was its stored GObject*
 * value.
 */
GLib::ObjectPtr::operator GObject* const() const
{
    return objectData;
}
