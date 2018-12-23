#pragma once
/**
 * @file  LibNM/BorrowedObjects/BorrowedObject.h
 *
 * @brief  A container for a LibNM object that is managed by NetworkManager
 *         and shouldn't be modified.
 */

#include "Nullable.h"
#include "JuceHeader.h"
#include <nm-object.h>

namespace LibNM { class BorrowedObject; }
namespace LibNM { class InnerObject; }


class LibNM::BorrowedObject : public Nullable<InnerObject*>
{
public:
    BorrowedObject();

    BorrowedObject(NMObject* nmObject);

    operator NMObject*() const;

    juce::String getPath();

    bool operator==(const BorrowedObject& rhs) const;

    bool operator==(const NMObject* rhs) const;

    bool operator!=(const BorrowedObject& rhs) const;

    bool operator!=(const NMObject* rhs) const;

    BorrowedObject& operator=(const BorrowedObject& rhs);

protected:
    void removeNMData();
};

class LibNM::InnerObject : public Nullable<NMObject*>
{
public:
    virtual ~InnerObject() { }

private:
    friend class BorrowedObject;

    InnerObject();

    InnerObject(NMObject* nmObject);

    operator NMObject*() const;
};
