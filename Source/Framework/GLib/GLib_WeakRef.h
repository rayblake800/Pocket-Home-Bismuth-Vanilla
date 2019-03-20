#pragma once
/**
 * @file  GLib_WeakRef.h
 *
 * @brief  A RAII object wrapper for GLib GWeakRef structures. 
 */

#include <glib-object.h>
#include "JuceHeader.h"

/**
 * @brief  Manages a GWeakRef data structure, used to safely access GObject
 *         data without preventing it from being destroyed.
 *
 *  Each WeakRef can serve as a weak reference to one GObject* at a time. When
 * requesting a GObject* held by a WeakRef, the WeakRef will only return the 
 * held value if the object is still valid. If the GObject data has been 
 * destroyed, it will instead return null. If the WeakRef returns a non-null 
 * value, the object's reference count will have been incremented, ensuring 
 * that the returned value will remain valid at least until the caller 
 * dereferences it.
 *
 *  WeakRef objects should never be used with GLib::Borrowed::Object classes,
 * as they cannot prevent a Borrowed::Object from being invalidated.
 */
namespace GLib { class WeakRef; }

class GLib::WeakRef
{
public:
    /**
     * @brief  Creates a weak reference with no initial data.
     */
    WeakRef();

    /**
     * @brief  Initializes the weak reference with a GObject value. 
     *
     * @param value  A GObject to hold a weak reference to, or NULL.
     */
    WeakRef(GObject* value);

    /**
     * @brief  Initializes the weak reference with another WeakRef's data.
     *
     * @param toAssign  A reference with data to share with the new WeakRef.
     */
    WeakRef(const WeakRef& toAssign);

    /**
     * @brief  Initializes the weak reference from a temporary WeakRef.
     *
     * @param toMove  A temporary WeakRef that will have its data moved to the
     *                new WeakRef.
     */
    WeakRef(WeakRef&& toMove);

    /**
     * @brief  Clears the reference on destruction.
     */
    ~WeakRef();

    /**
     * @brief  Assigns new object data to this weak reference.
     *
     * @param rhs  Another weak reference to copy.
     *
     * @return     This WeakRef.
     */
    WeakRef& operator=(const WeakRef& rhs);

    /**
     * @brief  Assigns new object data to this weak reference.
     *
     * @param rhs  A GObject to reference, or NULL to clear the current
     *             reference.
     *
     * @return     This WeakRef.
     */
    WeakRef& operator=(const GObject* rhs);

    /**
     * @brief  Checks if two WeakRefs reference the same data.
     *
     * @param rhs  Another WeakRef.
     *
     * @return     Whether this WeakRef and rhs both reference the same GObject
     *             or both hold NULL.
     */
    bool operator==(const WeakRef& rhs) const;

    /**
     * @brief  Checks if this object holds a specific GObject.
     *
     * @param rhs  A GObject to compare with this weak reference's data.
     *
     * @return     Whether this WeakRef's stored value equals rhs.
     */
    bool operator==(const GObject* rhs) const;
    
    /**
     * @brief  Attempts to return the GObject referenced by this WeakRef.  
     *
     * If a non-null value is returned, an additional strong reference will have 
     * been added to the returned GObject.
     *
     * @return  The GObject held by this weak reference, or NULL if this does 
     *          not reference an object, or the referenced object was destroyed.
     */
    GObject* getObject() const;

private:
    /**
     * @brief  Initializes the object's GWeakRef. Only constructors should call 
     *         this.
     *
     * @param initialValue  The initial object for the weak reference to track.
     */
    void initRef(const GObject* initialValue = nullptr);        

    /* The managed GWeakRef structure */
    GWeakRef weakRef;
    
    /* Tracks whether the GWeakRef has been initialized. */
    bool refInitialized = false;

    /* Tracks whether the GWeakRef has been cleared. */
    bool refCleared = false;

    /* Lock for reading when checking if the reference was initialized/cleared
       Lock for writing when initializing or clearing the reference. */
    juce::ReadWriteLock referenceLock;

    JUCE_LEAK_DETECTOR(GLib::WeakRef);
};
