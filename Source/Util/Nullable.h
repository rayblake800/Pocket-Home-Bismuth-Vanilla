#pragma once

/**
 * @file Nullable.h
 *
 * @brief  A template for classes that may or may not hold data.
 *
 * Nullable object containers are intended to package abstract pointer types.
 * 
 * Each Nullable class has a specific data pointer type.  Nullable classes
 * copy the interface of their data object, forwarding all function calls to
 * their object data pointer.  For each copied interface method, the nullable
 * class must check if its data pointer is null, and handle the problem 
 * appropriately if it is.
 *
 * This pattern provides all of the convenience of polymorphic data pointers, 
 * without the hassle of debugging segmentation faults or manually managing
 * memory.  In circumstances where a function returning a default value or 
 * occasionally being no-op would be inappropriate, Nullable objects should use
 * exceptions to ensure that unchecked function calls can be easily found and
 * fixed.
 *
 * @tparam PtrType  The type of data pointer held by the Nullable object.
 *                  This may be a normal pointer, an std::unique_ptr,
 *                  a juce::ReferenceCountedObjectPtr, or any sort of 
 *                  similar value that might equal nullptr.
 */

template<class PtrType>
Nullable
{
protected:
    Nullable(PtrType objectPtr) : objectPtr(objectPtr) { }

    Nullable() : objectPtr(nullptr) { }

public:
    virtual ~Nullable() { }

    bool isNull() const
    {
        return objectPtr == nullptr;
    }

protected:
    PtrType& getObject()
    {
        return objectPtr;
    }

private:
    PtrType objectPtr;
};
