// SharedPtr.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CTNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file SharedPtr.h
    @brief SharedPtr, intrusive reference counting for multiple pointer owners. */

#include <cassert>
#include <cstdlib>

/// Smart pointer for dynamic single object allocations on the heap.
#define Ptr(type) kNet::SharedPtr<type>

namespace kNet
{

/// Objects that require reference count tracking derive publicly from this.
/// \note The dtor of RefCountable is not virtual. NEVER manage pointers to RefCountables
///       and never delete a pointer to a RefCountable.
class RefCountable
{
public:
    RefCountable() : refCount(0) {}

    ~RefCountable()
    {
        refCount = -100000;  // Mark the refCount to an arbitrary negative value so that any increments or decrements
                             // will catch an assertion failure.
    }

    void AddRef()
    {
        assert(refCount >= 0);
        ++refCount;
    }

    void DecRef()
    {
        assert(refCount > 0);
        --refCount;
    }

    int RefCount() const { return refCount; }

private:
    int refCount;
};

/** @brief SharedPtr is an intrusive refcount-tracked single-object lifetime-manager.

SharedPtr wraps raw pointers to objects and manages reference
counting on the number of users on the pointer. When all users abandon the object,
FreeFunc::free(T*) is called to clean up the memory used by the resource. */
template <typename T>
class SharedPtr
{
public:
    /// Constructs a zero pointer.
    SharedPtr() : dataPtr(0) {}

    /// Constructs from a raw pointer.
    SharedPtr(T* ptr);

    /// Constructs from an existing SharedPtr.
    SharedPtr(const SharedPtr<T>& rhs);

    /// Template copy constructor for types that allow conversions to other types.
    template <typename T2>
    SharedPtr(const SharedPtr<T2>& rhs);

    /// Assignment from another SharedPtr.
    SharedPtr<T>& operator=(const SharedPtr<T>& rhs);

    /// Assignment from a raw ptr.
    SharedPtr<T>& operator=(T* ptr);

    // Releases use of the object.
    ~SharedPtr();

    unsigned int RefCount() const;  ///< Returns the reference count of the currently held object.

    const T* ptr() const;         ///< Const access to the pointer.
    const T* operator->() const;  ///< Const dereference and access object.
    const T& operator*() const;   ///< Const dereference object.

    operator T*();  ///< Implicit conversion to a raw pointer.

    T* ptr();         ///< Returns a pointer to the object or 0 if empty.
    T* operator->();  ///< Dereference and access object.
    T& operator*();   ///< Dereference object.

    operator bool() const;  ///< Test if pointer is good.

    operator bool();  ///< Test if pointer is good.

private:
    T* dataPtr;  ///< Pointer to the actual data.

    void AddRef();   ///< Increases reference count.
    void Release();  ///< Decreases reference count and deletes the object if 0.
};

/** Equality comparison for two SharedPtr. Note that the types must match.
    @param a First operand
    @param b Second operand
    @return true if the objects point to the same object, false otherwise. */
template <typename T>
bool operator==(const SharedPtr<T>& a, const SharedPtr<T>& b)
{
    if (a.ptr() == b.ptr())
        return true;
    else
        return false;
}

}  // ~kNet

#include "SharedPtr.inl"
