#pragma once
/**
 * @file  LibNM/BorrowedObjects/BorrowedObject.h
 *
 * @brief  A container for a LibNM object that is managed by NetworkManager
 *         and shouldn't be modified.
 */

#include "Nullable.h"
#include "JuceHeader.h"

namespace LibNM { template<typename NMType, class NMCreator>
        class BorrowedObject; }

namespace LibNM { template<typename NMType, class NMCreator>
        class InnerContainer; } 

/**
 *  BorrowedObject defines a container class for LibNM data pointers owned by 
 * NetworkManager. BorrowedObjects should only be used within the LibNM thread.
 * If NetworkManager destroys or invalidates the data pointer accessed through
 * the BorrowedObject, the BorrowedObject's held data will be set to null.
 *
 *  Only the BorrowedObject's designated NMCreator class objects may create
 * a BorrowedObject holding new LibNM data. Borrowed objects may be freely 
 * copied, reassigned, or created as null.
 *
 * @tparam NMCreator  The class responsible for creating and managing a 
 *                    BorrowedObject type.
 *
 * @tparam NMType     The LibNM data type stored in the BorrowedObject.
 */
template<typename NMType, class NMCreator>
class LibNM::BorrowedObject : 
public Nullable<typename InnerContainer<NMType, NMCreator>::Ptr>
{
private:
    /* The specific BorrowedObject type, renamed for brevity. */
    typedef typename Nullable<InnerContainer<NMType, NMCreator>>::Ptr 
            NullableType;

public:
    /**
     * @brief  Creates a null BorrowedObject.
     */
    BorrowedObject() { }
    
    BorrowedObject(const BorrowedObject& toCopy) : 
        NullableType(toCopy.getData()) { }

    operator NMType*()
    {
        if(NullableType::isNull())
        {
            return nullptr;
        }
        else if (NullableType::getData()->isNull())
        {
            NullableType::getDataReference() = nullptr;
            return nullptr;
        }
        return NullableType::getData()->getNMData();
    }

    BorrowedObject& operator=(const BorrowedObject& rhs)
    {
        NullableType::getDataReference() = rhs.getData();
    }

private:
    /* Only the NMCreator may create BorrowedObjects with new LibNM data or 
       remove the InnerContainer's LibNM data. */
    friend NMCreator;

    BorrowedObject(NMType* nmData) :
        NullableType(new InnerContainer<NMType, NMCreator>(nmData)) { }

    void clearData()
    {
        if(!NullableType::isNull())
        {
            NullableType::getData()->clearData();
            NullableType::getDataReference() = nullptr;
        }
    }
};


/**
 *  InnerContainer holds a single pointer to a LibNM object managed by 
 * NetworkManager. Each LibNM object is held by exactly one InnerContainer. That
 * InnerContainer may be referenced by any number of LibNM::BorrowedObject 
 * objects. InnerContainers may only be given a non-null value on construction. 
 * After construction, the container's held value may be changed to null, but 
 * it cannot be changed to any non-null value.
 * 
 *  Each InnerContainer class has a single designated NMType and NMCreator 
 * class. Only BorrowedObjects with the same NMType and NMCreator class may 
 * create non-null instances of that InnerContainer class. Only these 
 * BorrowedObjects objects are able to change the value held by their 
 * InnerContainers to null.
 *
 * @tparam NMType     The LibNM type held by an InnerContainer.
 *
 * @tparam NMCreator  The class responsible for creating and managing a
 *                    InnerContainer's BorrowedObject type.
 *
 */
template <typename NMType, class NMCreator>
class LibNM::InnerContainer : public Nullable<NMType*>,
    public juce::ReferenceCountedObject
{
public:
    virtual ~InnerContainer() { }

private:
    /* Pointer class used to manage reference counted InnerContainer object
       pointers. */
    typedef juce::ReferenceCountedObjectPtr<InnerContainer> Ptr;

    /* Only the corresponding BorrowedObject class may create and directly 
       access the InnerContainer. */
    friend class BorrowedObject<NMType, NMCreator>;
    
    /**
     * @brief  Gets the LibNM object data held by the InnerContainer.
     *
     * @return  The held LibNM data, or nullptr if the data is no longer valid.
     */
    NMType* getNMData() const
    {
        return Nullable<NMType*>::getData();
    }

    /**
     * @brief  Create an InnerContainer holding LibNM object data.
     *
     * @param nmData  A valid LibNM data pointer.
     */
    InnerContainer(NMType* nmData) : Nullable<NMType*>(nmData) { }

    /**
     * @brief  Permanently sets the InnerContainer's data pointer to null.
     */
    void clearData()
    {
        Nullable<NMType*>::getDataReference() = nullptr;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InnerContainer)
};
