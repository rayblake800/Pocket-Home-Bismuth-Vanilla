#pragma once
#include "LibNM/LibNM.h"
#include "GLib/Object.h"
#include <nm-object.h>

/**
 * @file  LibNM/NMObjects/Object.h
 *
 * @brief  The base class for all LibNM GObject classes.
 *
 *  LibNM::Object classes each contain a different subtype of libnm-glib's
 * base NMObject type. NMObject* values each represent a different connection
 * to a NetworkManager interface over DBus. Because libnm-glib is not guaranteed
 * to be threadsafe, all interaction with LibNM::Object and its subclasses
 * should occur on the GLib global default context loop, which runs on the
 * NMThread SharedResource object.
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
     * @param nmType    The new object's specific LibNM object type.
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

/**
 * @brief  A debugging macro used to find inappropriate LibNM::Object 
 *         access outside of the global default main context. In release builds,
 *         these checks will be removed.
 * 
 * LibNM::Object classes should call this at the beginning of most methods.
 */
#ifdef JUCE_DEBUG
#define ASSERT_CORRECT_CONTEXT                                \
    if(!g_main_context_is_owner(g_main_context_default()))    \
    {                                                         \
        DBG("LibNM: Accessed NMObject outside of NMThread!"); \
        jassertfalse;                                         \
    }
#else
#define ASSERT_CORRECT_CONTEXT
#endif
