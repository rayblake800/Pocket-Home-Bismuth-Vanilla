#pragma once

/**
 * @file  GLib/Borrowed/ObjectLender.h
 *
 * @brief  Manages a set of objects that share a Borrowed::Object subclass
 */

#include "JuceHeader.h"
#include <map>
#include <glib-object.h>

namespace GLib { namespace Borrowed { template <class BorrowedClass>
    class ObjectLender; } }

    /**
     * @tparam BorrowedClass  The Borrowed::Object subclass shared by this 
     *                        ObjectLender's managed objects.
     */
template <class BorrowedClass>
class GLib::Borrowed::ObjectLender
{
private:
    std::map<GObject*, BorrowedClass> borrowedObjects;

public:
    ObjectLender() { }

    /**
     * @brief  Invalidates all managed objects when the ObjectLender is 
     *         destroyed.
     */
    virtual ~ObjectLender()
    {
        for(auto& iter : borrowedObjects)
        {
            iter.second.invalidateObject();
        }
        borrowedObjects.clear();
    }

    /**
     * @brief  Creates a BorrowedClass object for an object data pointer if the
     *         ObjectLender isn't already managing that data pointer.
     *
     * @param objectData  A data pointer that should be wrapped in a 
     *                    BorrowedClass object managed by this ObjectLender.
     */
    void addIfNotAlreadyThere(GObject* objectData)
    {
        if(objectData != nullptr && borrowedObjects[objectData].isNull())
        {
            borrowedObjects[objectData].setGObject(objectData);
        }
    }

    /**
     * @brief  Gets a BorrowedClass object for an object data pointer, adding
     *         the data pointer to the list of values managed by the 
     *         ObjectLender if it isn't already there.
     *
     * @param objectData  A data pointer that needs to be wrapped in a
     *                    BorrowedClass object.
     *
     * @return            A BorrowedClass that holds objectData, or a null
     *                    BorrowedClass if objectData is not a valid GLib type.
     */
    BorrowedClass borrowObject(GObject* objectData)
    {
        if(objectData == nullptr)
        {
            return BorrowedClass();
        }
        addIfNotAlreadyThere(objectData);
        return borrowedObjects[objectData];
    }

    /**
     * @brief  Searches for a BorrowedClass object for an object data pointer,
     *         without changing the list of values managed by the ObjectLender.
     *
     * @param objectData  A data pointer to find within a managed BorrowedClass
     *                    object.
     *
     * @return            A BorrowedClass holding objectData, or a null
     *                    BorrowedClass if objectData isn't found.
     */
    BorrowedClass findObject(GObject* objectData) const
    {
        if(objectData == nullptr)
        {
            return BorrowedClass();
        }
        try
        {
            return borrowedObjects.at(objectData);
        }
        catch(std::out_of_range e)
        {
            return BorrowedClass();
        }
    }

    /**
     * @brief  Checks if an object data pointer is currently managed by this
     *         ObjectLender.
     *
     * @param objectData  The GObject* to search for in the list of managed
     *                    objects.
     *
     * @return            Whether objectData is a valid object managed by this
     *                    ObjectLender.
     */
    bool hasObject(GObject* objectData) const
    {
        try
        {
            return objectData != nullptr
                && !borrowedObjects.at(objectData).isNull();
        }
        catch(std::out_of_range e)
        {
            return false;
        }
    }

    /**
     * @brief  Invalidates an object data pointer, removing it from every
     *         BorrowedClass object that holds it. 
     *
     * @param objectData  An object data pointer that is no longer valid.
     */
    void invalidateObject(GObject* objectData)
    {
        if(borrowedObjects.count(objectData) > 0)
        {
            borrowedObjects[objectData].invalidateObject();
        }
    }

    /**
     * @brief  Gets a list containing one BorrowedClass object for each object
     *         data pointer managed by this ObjectLender.
     *
     * @return  The complete BorrowedClass object list.
     */
    juce::Array<BorrowedClass> getAllBorrowed() const
    {
        juce::Array<BorrowedClass> allBorrowed;
        for(const auto& iter : borrowedObjects)
        {
            allBorrowed.add(iter.second);
        }
        return allBorrowed;
    }

    /**
     * @brief  Invalidates all objects managed by the ObjectLender, setting
     *         every BorrowedClass instance that shares their data to null.
     */
    void invalidateAll()
    {
        for(auto& iter : borrowedObjects)
        {
            iter.second.invalidateObject();
        }
        borrowedObjects.clear();
    }
};
