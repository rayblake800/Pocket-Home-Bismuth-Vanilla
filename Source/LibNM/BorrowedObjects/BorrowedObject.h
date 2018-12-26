#pragma once
/**
 * @file  LibNM/BorrowedObjects/BorrowedObject.h
 *
 * @brief  A container for a LibNM object that is managed by NetworkManager
 *         and shouldn't be modified.
 */

#include "LibNM/BorrowedObjects/NMContainer.h"

namespace LibNM { template<class NMCreator, typename NMType>
    class BorrowedObject; }


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
template<class NMCreator, typename NMType>
class LibNM::BorrowedObject : 
public Nullable<typename NMContainer<NMCreator, NMType>::Ptr>
{
private:
    /* The specific BorrowedObject type, renamed for brevity. */
    typedef typename Nullable<NMContainer<NMCreator, NMType>>::Ptr NullableType;

public:
    /**
     * @brief  Creates a null BorrowedObject.
     */
    BorrowedObject() { }
    
    BorrowedObject(const BorrowedObject& toCopy) : 
        NullableType(toCopy.getData()) { }

    operator NMType*()
    {
        return (NullableType::isNull() ? nullptr :
                NullableType::getData()->getNMData());
    }

    BorrowedObject& operator=(const BorrowedObject& rhs)
    {
        NullableType::getDataReference() = rhs.getData();
    }

private:
    /* Only the NMCreator may create BorrowedObjects with a new NMContainer or
       remove the NMContainer's LibNM data. */
    friend NMCreator;

    BorrowedObject(typename NMContainer<NMCreator, NMType>::Ptr newData) :
        NullableType(newData) { }
};
