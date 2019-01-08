#pragma once
/**
 * @file GLib_ObjectPtr.h
 *
 * @brief  An immutable smart pointer used to access the internal GObject data 
 *         stored in a GLib::Object, automatically unreferencing it on
 *         destruction if necessary.
 */

#include <glib-object.h>

namespace GLib { class Object; }
namespace GLib { class ObjectPtr; }

/**
 *  GLib::ObjectPtr provides a way to directly access a GLib::Object's stored
 * data pointer without having to manually manage GLib reference counts. 
 * ObjectPtrs are only intended for temporary use and should not be created
 * outside of function scope.
 */
class GLib::ObjectPtr
{
public:
    /**
     * @brief  Creates this ObjectPtr to access a GLib::Object's internal 
     *         GObject* data.
     *
     * @param dataSource  The Object with data the ObjectPtr will access.
     */
    ObjectPtr(const Object& dataSource);

    /**
     * @brief  Creates an ObjectPtr to hold a GObject* value.
     *
     * This constructor assumes that the dataSource will need to be unreferenced
     * on destruction, as otherwise there's no reason to store it in an 
     * ObjectPtr.
     *
     * @param dataSource  A GLib object data pointer the ObjectPtr will hold.
     */
    ObjectPtr(GObject* dataSource);

    /**
     * @brief  If necessary, unreferences the stored GObject* value.
     */
    ~ObjectPtr();

    /**
     * @brief  Allows this ObjectPtr to be directly used as if it was its
     *         stored GObject* value.
     *
     * @return   The pointer's GObject* value.
     */
    operator GObject* const() const;

private:
    GObject* const objectData;
    const bool shouldUnreference;
};
