#pragma once
/**
 * @file  LibNM/BorrowedObjects/BorrowedObjectFactory.h
 *
 * @brief  Creates LibNM::BorrowedObject objects, and ensures that they only
 *         remain available while they are still valid.
 */

#include "JuceHeader.h"
#include <nm-object.h>
#include <map>

namespace LibNM { class BorrowedObjectFactory; }
namespace LibNM { class BorrowedObject; }
namespace LibNM { class InnerObject; }

class LibNM::BorrowedObjectFactory
{
public:
    BorrowedObjectFactory();

    ~BorrowedObjectFactory() { }

    class ObjectProvider
    {
    public:
        virtual ~ObjectProvider() { }

    protected:
        ObjectProvider(BorrowedObjectFactory& objectFactory) :
        objectFactory(objectFactory) { }

        BorrowedObject getBorrowedObject(NMObject* objectData);

        void addNMData(NMObject* borrowedObject);

        void removeNMData(NMObject* removedObject);

        BorrowedObjectFactory& objectFactory;
    };

private:
    std::map<NMObject*, InnerObject> objectTracker; 
};
