#pragma once
#include <glib.h>
#include <map>
#include "JuceHeader.h"
#include "GPPObject.h"
/**
 * @file GSignalHandler.h
 * 
 * @brief Responds to signals emitted by GLib objects.
 */

template <class GPPObjectType>
class GSignalHandler : public GPPObject<GPPObjectType>::SignalHandler
{    
protected:
    GSignalHandler() { }

    /**
     * Copies an existing signal handler, subscribing to all of that
     * handler's tracked signals.
     * 
     * @param rhs  Another valid signal handler.
     */
    GSignalHandler(const GSignalHandler<GPPObjectType>& rhs)
    {
        juce::Array<GObject*> signalSources;
        const juce::ScopedLock readSourceLock(rhs.signals.getLock());
        auto iter = rhs.signals.begin();
        while(iter.next())
        {
            GObject* source = iter.getKey();
            if(source != nullptr)
            {
                g_object_ref(iter.getKey());
                signalSources.add(source);
            }
        }
        const juce::ScopedUnlock readFinished(rhs.signals.getLock());
        
        for(GObject* source : signalSources)
        {
            GPPObjectType tempHolder(source);
            tempHolder.connectSignalHandler(*this);
            g_object_unref(source);
        }
    }

public:
    /**
     * Unsubscribes the signal handler from all signal sources, and removes all
     * held references to signal sources.
     */
    virtual ~GSignalHandler()
    {
        unsubscribeAll();
    }
    
protected:
    /**
     * Unsubscribes this signal handler from all signals emitted by a GObject.
     * 
     * @param source  A GObject signal source.
     */
    void disconnectSignals(GObject* source)
    {
        const juce::ScopedLock changeSourceLock(signals.getLock());
        if(source != nullptr && signals.contains(source));
        {
            for(const guint& signalID : signals[source])
            {
                g_signal_handler_disconnect(source, signalID);
            }
            g_object_unref(source);
            signals.remove(source);
        }       
    }
    
    /**
     * Unsubscribes the signal handler from all signal sources, and removes all
     * held references to signal sources.
     */
    void unsubscribeAll()
    {
        const juce::ScopedLock changeSourceLock(signals.getLock());
        auto iter = signals.begin();
        while(iter.next())
        {
            GObject* source = iter.getKey();
            if(source == nullptr)
            {
                continue;
            }
            for(const guint& signalID : iter.getValue())
            {
                g_signal_handler_disconnect(source, signalID);
            }
            g_object_unref(source);
        }   
        signals.clear();
    }
    
private:
            
    /**
     * Subscribe the signal handler to a single signal.
     * 
     * @param source      A GObject signal source.  The signal handler will
     *                    acquire a reference to the source's GObject, if it 
     *                    doesn't have one already.
     * 
     * @param signalName  The signal that handler should receive.
     * 
     * @param callback    A signal-specific callback function.  This should
     *                    be a static void function that takes parameters 
     *                    specific to the subscribed signal.  A pointer to 
     *                    this signal handler will be passed to the callback
     *                    as data.
     */
    virtual void connectSignal(GObject* source,
            const char* signalName, GCallback callback) final override
    {
        if(source == nullptr || signalName == nullptr)
        {
            return;
        }
        const juce::ScopedLock signalLock(signals.getLock());
        gulong handlerID = g_signal_connect
                (source, signalName, callback, this);
        if(handlerID != 0)
        {
            juce::Array<gulong>& existingSignals = signals.getReference(source);
            if(existingSignals.isEmpty())
            {
                g_object_ref(source);
            }
            existingSignals.add(handlerID);
        }       
    }
    
    /**
     * Maps each GObject* signal source to the list of subscribed signal IDs 
     * that belong to this SignalHandler.  The SignalHandler holds a reference 
     * to each GObject* here, ensuring that all of them remain valid.
     */
    juce::HashMap<GObject*, juce::Array<gulong>,
            juce::DefaultHashFunctions, juce::CriticalSection> signals;
};
