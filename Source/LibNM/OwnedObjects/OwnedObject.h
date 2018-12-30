#pragma once
/**
 * @file  LibNM/OwnedObjects/OwnedObject.h
 *
 * @brief  A base class for managing owned LibNM GObject data pointers.
 */

#include "GLib/Object.h"
#include <nm-object.h>

namespace LibNM { class OwnedObject; }

/**
 *  LibNM::Object classes each contain a different subtype of libnm-glib's
 * base NMObject type. NMObject* values each represent a different connection
 * to a NetworkManager interface over DBus. Because libnm-glib is not guaranteed
 * to be threadsafe, all interaction with LibNM::Object and its subclasses
 * should occur on the GLib global default context loop, which runs on the
 * NMThread SharedResource object.
 */
class LibNM::OwnedObject : public GLib::Object
{
protected:
    /**
     * @brief  Creates a null LibNM object.
     *
     * @param nmType  The new object's specific LibNM object type.
     */
    OwnedObject(const GType nmType);

    /**
     * @brief  Creates a LibNM object sharing data with an existing LibNM
     *         object.
     *
     * @param toCopy  The LibNM::Object instance to copy.
     *
     * @param nmType  The new object's specific LibNM object type.
     */
    OwnedObject(const OwnedObject& toCopy, const GType nmType);

    /**
     * @brief  Creates a LibNM object to hold a NMObject pointer.
     *
     * @param toAssign  An NMObject* that the new Object will manage.
     *
     * @param nmType    The new object's specific LibNM object type.
     */
    OwnedObject(const NMObject* toAssign, const GType nmType);

public:
    /**
     * @brief  Gets the Object's DBus path.
     *
     * @return  The DBus path to the Object's underlying NetworkManager DBus
     *          object, or the empty string if the object is null.
     */
    const char* getPath() const;
};
