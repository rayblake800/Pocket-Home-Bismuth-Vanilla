#pragma once
/**
 * @file  GLib_SharedContextPtr.h
 *
 * @brief  Creates and shares a GLib context data structure.
 */

#include <gio/gio.h>

namespace GLib { class SharedContextPtr; }

/**
 * @brief  A managed GMainContext* that can be copied or set to new values,
 *         automatically referencing or unreferencing its GMainContext as
 *         necessary.
 */
class GLib::SharedContextPtr
{
public:
    /**
     * @brief  Creates a context pointer with a null value.
     */
    SharedContextPtr() { }

    /**
     * @brief  Creates a context pointer with an initial context value.
     *
     * @param context  The stored GLib context. If non-null, this should have
     *                 one reference added to it that the SharedContextPtr will
     *                 unreference on destruction or value change.
     */
    SharedContextPtr(GMainContext* context);

    /**
     * @brief  Creates a context pointer using another context pointer's
     *         GMainContext* value.
     *
     * @param contextPtr  Any other context pointer. If this contains a non-null
     *                    GMainContext*, the new context pointer will copy and
     *                    reference it.
     */
    SharedContextPtr(const SharedContextPtr& contextPtr);

    /**
     * @brief  Creates a context pointer using data taken from a temporary
     *         context pointer.
     *
     * @param contextPtr  A temporary context pointer the new context pointer
     *                    will take data from.
     */
    SharedContextPtr(SharedContextPtr&& contextPtr);

    /**
     * @brief  Unreferences the internal GMainContext* on destruction if it
     *         holds a non-null value.
     */
    virtual ~SharedContextPtr();

    /**
     * @brief  Checks if this context pointer holds a specific GMainContext.
     *
     * @param rhs  A GMainContext* to compare with the SharedContextPtr's
     *             internal GMainContext pointer.
     *
     * @return     Whether rhs equals the context pointer's internal
     *             GMainContext*.
     */
    bool operator== (const GMainContext* rhs) const;

    /**
     * @brief  Checks if this context pointer and another hold identical
     *         GMainContext pointers.
     *
     * @param rhs  Another context pointer to compare with this one.
     *
     * @return     Whether this context pointer and rhs hold identical
     *             GMainContext* values.
     */
    bool operator== (const SharedContextPtr& rhs) const;

    /**
     * @brief  Checks if this context pointer does not hold a specific
     *         GMainContext.
     *
     * @param rhs  A GMainContext* to compare with the SharedContextPtr's
     *             internal GMainContext pointer.
     *
     * @return     Whether rhs does not equal the context pointer's internal
     *             GMainContext*.
     */
    bool operator!= (const GMainContext* rhs) const;

    /**
     * @brief  Checks if this context pointer and another do not hold identical
     *         GMainContext pointers.
     *
     * @param rhs  Another context pointer to compare with this one.
     *
     * @return     Whether this context pointer and rhs do not hold identical
     *             GMainContext* values.
     */
    bool operator!= (const SharedContextPtr& rhs) const;

    /**
     * @brief  Assigns a new GLib context pointer to this SharedContextPtr.
     *
     *  If this assignment replaces a non-null GMainContext*, the old
     * GMainContext* will be unreferenced before it is removed.
     *
     * @param rhs  A new GMainContext* for the SharedContextPtr to hold. If
     *             non-null, rhs should have an added reference that the
     *             SharedContextPtr will later unreference.
     *
     * @return     A reference to this SharedContextPtr.
     */
    SharedContextPtr& operator= (GMainContext* const rhs);

    /**
     * @brief  Assigns a new GLib context pointer to this SharedContextPtr.
     *
     *  If this assignment replaces a non-null GMainContext*, the old
     * GMainContext* will be unreferenced before it is removed.
     *
     * @param rhs  Another SharedContextPtr, holding the GMainContext* value
     *             that will be assigned to this context pointer. If non-null,
     *             a new reference will be added to the copied GMainContext*
     *             that the SharedContextPtr will later unreference.
     *
     * @return     A reference to this SharedContextPtr.
     */
    SharedContextPtr& operator= (const SharedContextPtr& rhs);

    /**
     * @brief  Gets this SharedContextPtr's internal GMainContext* value.
     *
     * @return  The stored context pointer.
     */
    GMainContext* operator*() const;

private:
    // The internal GLib context pointer.
    GMainContext* context = nullptr;
};
