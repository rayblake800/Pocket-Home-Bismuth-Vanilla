#pragma once
#include "SharedResource/SharedResource.h"

/**
 * @file  SharedResource/Implementation/Implementation.h
 *
 * @brief  Declares the private classes used to implement the SharedResource
 *         module.
 */
namespace SharedResource
{
    /**
     * @brief  A singleton object that holds all SharedResource object instances
     *         and locks.
     *
     * @see  SharedResource/Implementation/Holder.h
     */
    class Holder;

    /**
     * @brief  The parent class of all SharedResource::Resource objects, used
     *         to track the connections between the Resouce and its handlers.
     *
     * @see  SharedResource/Implementation/Instance.h
     */
    class Instance;

    /**
     * @brief  The parent class of all SharedResource::Handler objects, used to
     *         connect to the resource Instance and manage its life cycle.
     *
     * @see  SharedResource/Implementation/Reference.h
     */
    class Reference;

    /**
     * @brief  An object used as both a pointer to a resource Instance that
     *         that keeps the resource locked for as long as it exists.
     *
     * @see  SharedResource/Implementation/LockedInstancePtr.cpp
     */
    class LockedInstancePtr;
}

