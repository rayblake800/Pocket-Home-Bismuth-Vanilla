#pragma once
#include "LibNM/LibNM.h"
#include "GLib/Object.h"
#include <nm-object.h>

/**
 * @file  LibNM/NMObjects/Object.h
 *
 * @brief  The base class for all LibNM GObject classes.
 */
class LibNM::Object : public GLib::Object
{
protected:
    /**
     * @brief  Creates a null LibNM object.
     *
     * @param nmType  The new object's specific LibNM object type.
     */
    Object(const GType nmType);

    /**
     * @brief  Creates a LibNM object sharing data with an existing LibNM
     *         object.
     *
     * @param toCopy  The LibNM::Object instance to copy.
     *
     * @param nmType  The new object's specific LibNM object type.
     */
    Object(const Object& toCopy, const GType nmType);

    /**
     * @brief  Creates a LibNM object to hold a NMObject pointer.
     *
     * @param toAssign  An NMObject* that the new Object will manage.
     *
     * @param nmType  The new object's specific LibNM object type
     */
    Object(const NMObject* toAssign, const GType nmType);

public:
    /**
     * @brief  Gets the Object's DBus path.
     *
     * @return  The DBus path to the Object's underlying NetworkManager DBus
     *          object, or the empty string if the object is null.
     */
    const char* getPath() const;
};
