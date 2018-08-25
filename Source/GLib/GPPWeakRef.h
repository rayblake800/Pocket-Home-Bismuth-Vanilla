#pragma once
#include <glib-object.h>
#include "JuceHeader.h"

/**
 * @file GPPWeakRef.h
 *
 * @brief A RAII object wrapper for GLib GWeakRef structures. 
 */
class GPPWeakRef
{
public:

    GPPWeakRef();

    /**
     * Initialize the weak reference with a GObject value. 
     *
     * @param value  A GObject to hold a weak reference to, or NULL.
     */
    GPPWeakRef(GObject* value);

    ~GPPWeakRef();

    /**
     * Assign new object data to this weak reference.
     *
     * @param rhs  Another weak reference to copy.
     *
     * @return     This GPPWeakRef.
     */
    GPPWeakRef& operator=(const GPPWeakRef& rhs);

    /**
     * Assigns new object data to this weak reference.
     *
     * @param rhs  A GObject to reference, or NULL to clear the current
                   reference.
     *
     * @return     This GPPWeakRef.
     */
    GPPWeakRef& operator=(const GObject* rhs);

    /**
     * Checks if two GPPWeakRefs reference the same data.
     *
     * @param rhs  Another GPPWeakRef.
     *
     * @return  True iff this GPPWeakRef and rhs both reference the same
     *          GObject, or both hold NULL.
     */
    bool operator==(const GPPWeakRef& rhs) const;

    /**
     * Checks if this object holds a specific GObject.
     *
     * @param rhs  A GObject to compare with this weak reference's data.
     *
     * @return  True iff this GPPWeakRef's stored value equals rhs.
     */
    bool operator==(const GObject* rhs) const;

    /**
     * Casts stored object pointer data to an unsigned long, so GPPWeakRefs
     * can be used as hash keys.
     *
     * @return  The referenced data address, as an unsigned 64-bit number.
     */
    operator juce::uint64() const;

    
    /**
     * Attempts to return the GObject referenced by this GPPWeakRef.  If a
     * non-null value is returned, an additional strong reference will have been
     * added to the returned GObject.
     *
     * @return  The GObject held by this weak reference, or NULL if this
     *          does not reference an object, or the referenced object was
     *          destroyed.
     */
    GObject* getObject() const;

private:
    GWeakRef weakRef;
};
