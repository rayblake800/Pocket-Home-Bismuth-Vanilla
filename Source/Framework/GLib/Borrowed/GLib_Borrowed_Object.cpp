#include "GLib_Borrowed_Object.h"

using BorrowedObject = GLib::Borrowed::Object;

/*
 * Creates a null Object, with no internal GObject.
 */
BorrowedObject::Object(const GType objectType) :
    GLib::Object(objectType) { }

/*
 * Creates a new Object as a reference to existing object data.
 */
BorrowedObject::Object(const Object& toCopy, const GType objectType) :
    GLib::Object(objectType), objectContainer(toCopy.objectContainer) { }

BorrowedObject::~Object() { }


/*
 * Sets this Object's data container to a new reference of another Object's 
 * stored GObject*.
 */
GLib::Borrowed::Object& BorrowedObject::operator=(const Object& rhs)
{
    setGObject(rhs);
    return *this;
}

/*
 * Gets a pointer to this object's data.
 */
GObject* BorrowedObject::getGObject() const 
{
    if(objectContainer == nullptr || objectContainer->isNull())
    {
        return nullptr;
    }
    return *objectContainer;
}

/*
 * Assigns new GObject* data to this Object.
 */
void BorrowedObject::setGObject(const GLib::Object& toCopy)
{
    const Object* borrowedCopy = dynamic_cast<const Borrowed::Object*>(&toCopy);
    if(borrowedCopy != nullptr && g_type_is_a(toCopy.getType(), getType()))
    {
        objectContainer = borrowedCopy->objectContainer;
    }
}

/*
 * Clears all held GObject* data from this Object instance.
 */
void BorrowedObject::clearGObject()
{
    objectContainer = nullptr;
}

/*
 * Assigns new GObject data to this Object. 
 */
void BorrowedObject::setGObject(GObject* toAssign)
{
    if(toAssign == nullptr)
    {
        objectContainer = nullptr;
    }
    else
    {
        objectContainer = new SharedContainer(toAssign);
    }
}

/*
 * Removes this object's stored GObject* data from every borrowed Object that
 * contains that data.
 */
void BorrowedObject::invalidateObject()
{
    if(objectContainer != nullptr)
    {
        objectContainer->clearData();
        objectContainer = nullptr;
    }
}
