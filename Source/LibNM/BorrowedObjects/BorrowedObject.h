#pragma once
/**
 * @file  LibNM/BorrowedObjects/BorrowedObject.h
 *
 * @brief  Defines, creates and manages object containers/interfaces for LibNM 
 *         object data pointers owned by the NetworkManager.
 */

#include "Nullable.h"
#include "JuceHeader.h"
#include <map>

namespace LibNM { template<typename NMType> class BorrowedObjectInterface; }
namespace LibNM { template<typename NMType> class BorrowedObjectSet; }
namespace LibNM { template<typename NMType> class BorrowedObject; }
namespace LibNM { template<typename NMType> class InnerContainer; } 

template<typename NMType>
class LibNM::BorrowedObjectInterface
{
private:
    BorrowedObject<NMType> objectData;

public:
    BorrowedObjectInterface() { }

    BorrowedObjectInterface(BorrowedObject<NMType> initialData) :
        objectData(initialData) { }

    virtual ~BorrowedObjectInterface() { }

    const char* getPath() const
    {
        if(isNull())
        {
            return "";
        }
        return nm_object_get_path(NM_OBJECT(getNMData()));
    }

    bool isNull() const
    {
        return objectData.isNull();
    }

    NMType* getNMData() const
    {
        return (NMType*) objectData;
    }
};

/**
 *  Creates and manages a collection of BorrowedObject instances for a single
 * type of LibNM object data. This list will never contain more than one
 * BorrowedObject with the same data pointer. All BorrowedObject instances
 * created by a BorrowedObjectSet will be set to null when the BorrowedObjectSet
 * is destroyed.
 *
 * @tparam NMType  The type of LibNM object data held by the BorrowedObject
 *                 instances.
 */
template<typename NMType>
class LibNM::BorrowedObjectSet
{
private:
    /* Store objects mapped to their LibNM pointer values to allow the set to
       quickly find if a NMType* already has an associated BorrowedObject. */
    std::map<NMType*, BorrowedObject<NMType>> objectMap;

public:
    BorrowedObjectSet() { }

    /**
     * @brief  Clear all BorrowedObject instances created directly or indirectly
     *         by this BorrowedObjectSet.
     */
    virtual ~BorrowedObjectSet() 
    { 
        for(auto& borrowed : objectMap)
        {
            borrowed.second.clearData();
        }
        objectMap.clear();
    }

    /**
     * @brief  Gets a BorrowedObject for a specific LibNM data pointer.
     *
     * @param nmData  The LibNM object data pointer used to either find or
     *                create a new BorrowedObject.
     *
     * @return        A BorrowedObject holding the nmData value.
     */
    BorrowedObject<NMType> getBorrowedObject(NMType* nmData)
    {
        if(objectMap.count(nmData) == 0
                || objectMap[nmData].isNull())
        {
            objectMap[nmData] = BorrowedObject<NMType>(nmData);
        }
        return objectMap[nmData];
    }

    BorrowedObject<NMType> findBorrowedObject(NMType* nmData) const
    {
        try
        {
            return objectMap.at(nmData);
        }
        catch(std::out_of_range e)
        {
            return BorrowedObject<NMType>();
        }
    }

    /**
     * @brief  Sets BorrowedObject instances with a specific data pointer to
     *         null. 
     *
     * @param nmData  A LibNM object data pointer that is no longer valid.
     *
     *  This affects the single BorrowedObject created by the BorrowedObjectSet 
     * to hold the nmData pointer, all copies of that original object, all 
     * copies of those copies, and so on. To ensure that this clears absolutely 
     * every BorrowedObject holding the nmData pointer, only one 
     * BorrowedObjectSet should exist that has access to the nmData pointer.
     */
    void removeBorrowedObject(NMType* nmData)
    {
        if(objectMap.count(nmData) > 0 || objectMap[nmData].isNull())
        {
            objectMap[nmData].clearData();
            objectMap[nmData] = nullptr;
        }
    }
    
    /**
     * @brief  Gets all non-null BorrowedObject instances held by the
     *         BorrowedObjectSet.
     *
     * @return  A list containing one BorrowedObject instance for each valid 
     *          LibNM object data pointer tracked by the BorrowedObjectSet.
     */
    juce::Array<BorrowedObject<NMType>> getAll() const
    {
        juce::Array<BorrowedObject<NMType>> validObjects;
        for(const auto& mappedObject : objectMap)
        {
            if(!mappedObject.second.isNull())
            {
                validObjects.add(mappedObject.second);
            }
        }
        return validObjects;
    }
};

/**
 *  InnerContainer holds a single pointer to a LibNM object managed by 
 * NetworkManager. Each LibNM object is held by exactly one InnerContainer. That
 * InnerContainer may be referenced by any number of BorrowedObject instances.
 * This allows the BorrowedObjectSet to remove all references to a LibNM data
 * pointer by clearing a single InnerContainer object.
 *
 *  InnerContainers may be set to a non-null value only during construction.  
 * After construction, the container's held value may be changed to null, but it
 * cannot be changed to any non-null value. InnerContainer objects are reference
 * counted, and will be automatically deleted when they are no longer held by
 * the BorrowedObjectSet or any BorrowedObject.
 *
 * @tparam NMType     The LibNM type held by an InnerContainer.
 *
 */
template <typename NMType>
class LibNM::InnerContainer : public Nullable<NMType*>,
    public juce::ReferenceCountedObject
{
public:
    virtual ~InnerContainer() { }

    /* Pointer class used to manage reference counted InnerContainer object
       pointers. */
    typedef juce::ReferenceCountedObjectPtr<InnerContainer> Ptr;
private:
    /* Only the corresponding BorrowedObject class may create and directly 
       access the InnerContainer. */
    friend class BorrowedObject<NMType>;
    
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

/**
 *  BorrowedObject is a container class template for LibNM data pointers owned 
 * by NetworkManager. Each BorrowedObject holds either a single LibNM data 
 * pointer or a null value. Unlike GLib::Object containers, a BorrowedObject 
 * will not alter the reference count of the data it holds. BorrowedObject 
 * classes should only be used within the LibNM thread. 
 *
 *   All BorrowedObject instances have a source BorrowedObjectSet. If 
 * NetworkManager destroys or invalidates a data pointer accessed through the 
 * BorrowedObject, that data pointer should be removed using the 
 * BorrowedObjectSet's removeBorrowedObject method. Calling this method will set
 * every BorrowedObject holding the data pointer to null, ensuring that all
 * references to invalidated data are removed.
 *
 *  Only BorrowedObjectSet objects may create BorrowedObject instances holding 
 * new LibNM data. Otherwise, borrowed objects may be freely copied, reassigned, 
 * or created as null. 
 *
 * @tparam NMType     The LibNM data type stored in the BorrowedObject.
 */
template<typename NMType>
class LibNM::BorrowedObject : 
public Nullable<typename InnerContainer<NMType>::Ptr>
{
private:
    /* The specific BorrowedObject type, renamed for brevity. */
    typedef Nullable<typename InnerContainer<NMType>::Ptr> NullableType;

public:
    /**
     * @brief  Creates a null BorrowedObject.
     */
    BorrowedObject() { }
    
    /**
     * @brief  Creates a BorrowedObject sharing another object's LibNM data.
     *
     * @param toCopy  Another BorrowedObject that will share LibNM data with the
     *                new object.
     */
    BorrowedObject(const BorrowedObject& toCopy) : 
        NullableType(toCopy.getData()) { }

    /**
     * @brief  Allow the BorrowedObject to be directly cast to its stored LibNM
     *         object data pointer.
     *
     * @return   The object's stored LibNM data pointer, or nullptr if no data
     *           is stored.
     */
    operator NMType*() const
    {
        if(NullableType::isNull() || NullableType::getData()->isNull())
        {
            return nullptr;
        }
        return NullableType::getData()->getNMData();
    }

    /**
     * @brief  Assigns another BorrowedObject's data to this BorrowedObject.
     *
     * @param rhs  The BorrowedObject data source.
     *
     * @return     This BorrowedObject.
     */
    BorrowedObject& operator=(const BorrowedObject& rhs)
    {
        NullableType::getDataReference() = rhs.getData();
        return *this;
    }

private:
    /* Only the BorrowedObjectSet may create BorrowedObject instances with new 
       LibNM object data or remove the InnerContainer's LibNM data. */
    friend BorrowedObjectSet<NMType>;

    /**
     * @brief  Creates a BorrowedObject to hold a specific LibNM object data
     *         pointer.
     *
     * @param nmData  The object data pointer the BorrowedObject will hold.
     */
    BorrowedObject(NMType* nmData) :
        NullableType(new InnerContainer<NMType>(nmData)) { }

    /**
     * @brief  Clears the LibNM data stored in this BorrowedObject, removing it
     *         from all BorrowedObject instances.
     */
    void clearData()
    {
        if(!NullableType::isNull())
        {
            NullableType::getData()->clearData();
            NullableType::getDataReference() = nullptr;
        }
    }
};

