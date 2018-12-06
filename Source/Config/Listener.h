#pragma once
/**
 * @file  Config/Listener.h
 *
 * @brief  A listener that tracks any number of values in a 
 *         Config::FileResource. 
 */

#include "Config/Implementation/ListenerInterface.h"
#include "SharedResource/Handler.h"

namespace Config { template <class ResourceClass> class Listener; }

template <class ResourceClass>
class Config::Listener : public ListenerInterface,
    protected SharedResource::Handler<ResourceClass>
{
private:
    /* Tracks all keys this listener follows. */
    juce::Array<juce::Identifier, juce::CriticalSection> subscribedKeys;

public:
    Listener() { }

    virtual ~Listener() { }

    /**
     * @brief Calls configValueChanged() for every key tracked by this listener.
     */
    void loadAllConfigProperties()
    {
        using juce::Identifier;
        juce::Array<Identifier> notifyKeys;
        {
            const juce::ScopedLock updateLock(subscribedKeys.getLock());
            notifyKeys.addArray(subscribedKeys);
        }
        for (const Identifier& key : notifyKeys)
        {
           configValueChanged(key);
        }
    }

    /**
     * @brief Adds a key to the list of keys tracked by this listener.
     *
     * @param keyToTrack  Whenever a value with this key is updated, the
     *                    Listener will be notified.
     */
    void addTrackedKey(const juce::Identifier& keyToTrack)
    {
        const juce::ScopedLock keyListLock(subscribedKeys.getLock());
        subscribedKeys.add(keyToTrack);
    }

    /**
     * @brief Unsubscribes from updates to a FileResource value.
     *
     * @param keyToRemove  This listener will no longer receive updates when
     *                     the value with this key is updated.
     */
    void removeTrackedKey(const juce::Identifier& keyToRemove)
    {
        const juce::ScopedLock keyListLock(subscribedKeys.getLock());
        subscribedKeys.removeAllInstancesOf(keyToRemove);
    }
    

    /**
     * @brief  Requests a stored value directly from this Listener's 
     *         FileResource.
     *
     * @tparam ValueType        The type of value requested.
     *
     * @param key               The key to the requested value.
     *
     * @throws BadKeyException  If the key does not map to a value with the 
     *                          correct type.
     *
     * @return                  The requested value.
     */
    template<typename ValueType>
    ValueType getConfigValue(const juce::Identifier& key)
    {
        SharedResource::LockedPtr<ResourceClass> configFile
            = SharedResource::Handler<ResourceClass>::getReadLockedResource();
        return configFile->template getConfigValue<ValueType>(key);
    }

private:
    /**
     * @brief  Checks if a particular value is tracked by this Listener.
     *
     * @param key  The key to a value stored in the Listener's file resource.
     *
     * @return     Whether the value with the given key is tracked by the 
     *             Listener.
     */
    virtual bool isKeyTracked(const juce::Identifier& key) const override
    {
        const juce::ScopedLock keyListLock(subscribedKeys.getLock());
        return subscribedKeys.contains(key);
    }

    /**
     * @brief  This method will be called whenever a key tracked by this 
     *         listener changes in the config file.
     *
     *  By default, this takes no action. Override this method to handle config 
     * value update events.
     * 
     * @param propertyKey   Passes in the updated value's key.
     */
    virtual void configValueChanged(const juce::Identifier& propertyKey) 
        override { }
};
