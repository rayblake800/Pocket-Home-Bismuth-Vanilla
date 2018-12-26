#pragma once
/**
 * @file  LibNM/BorrowedObjects/NMContainer.h
 *
 * @brief  The container class used to hold a LibNM object, shared between 
 *         multiple BorrowedObject objects.
 */

#include "Nullable.h"
#include "JuceHeader.h"

namespace LibNM { template <class NMCreator, typename NMPtrType> 
    class NMContainer; }

/**
 *  NMContainer holds a single pointer to a LibNM object managed by 
 * NetworkManager. Each LibNM object is held by exactly one NMContainer. That
 * NMContainer may be referenced by any number of LibNM::BorrowedObject objects.
 * NMContainers may only be given a non-null value on construction. After
 * construction, the container's held value may be changed to null, but 
 * it cannot be changed to any non-null value.
 * 
 *  Each NMContainer class has a single designated NMCreator class. Only
 * NMCreator objects may create non-null instances of that NMContainer class.
 * Only NMCreator objects are able to change the value held by their
 * NMContainers to null.
 *
 *  Data held in NMContainers may be destroyed by NetworkManager. This
 * is guaranteed to only occur within the LibNM::ThreadResource thread, but
 * otherwise may occur at any time. When this occurs, the NMCreator object
 * that created the NMContainer is responsible for detecting the change, and
 * setting the NMContainer to null. Since all BorrowedObjects accessing that
 * LibNM data access it through the same NMContainer, this ensures that every
 * single reference to the invalidated LibNM data is properly removed.
 *
 * @tparam NMCreator  The class responsible for creating and managing a
 *                    NMContainer type.
 *
 * @tparam NMType     The LibNM type held by a NMContainer.
 */
template <class NMCreator, typename NMType>
class LibNM::NMContainer : public Nullable<NMType*>,
    public juce::ReferenceCountedObject
{
public:
    /**
     * @brief  Creates a null, immutable NMContainer object.
     */
    NMContainer() { }

    virtual ~NMContainer() { }

    /**
     * @brief  Gets the LibNM object data held by the NMContainer.
     *
     * @return  The held LibNM data, or nullptr if the data is no longer valid.
     */
    NMType* getNMData() const
    {
        return Nullable<NMType*>::getData();
    }

    /* Pointer class used to hold NMContainer object pointers. */
    typedef juce::ReferenceCountedObjectPtr<NMContainer> Ptr;

private:
    /* Only the NMCreator can construct non-null NMContainers, and clear
       NMContainer data. */
    friend NMCreator;

    /**
     * @brief  Create an NMContainer holding LibNM object data.
     *
     * @param nmData  A valid LibNM data pointer.
     */
    NMContainer(NMType* nmData) : Nullable<NMType*>(nmData) { }

    /**
     * @brief  Permanently sets the NMContainer's data pointer to null.
     */
    void clearData()
    {
        Nullable<NMType*>::getDataReference() = nullptr;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NMContainer)
};
