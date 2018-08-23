#include "GSignalHandler.h"

/*
 * Unsubscribes the signal handler from all signal sources, and removes all
 * held references to signal sources.
 */
virtual GSignalHandler::~GSignalHandler()
{
    unsubscribeAll();
}

/*
 * Subscribe the signal handler to a single signal.
 */
void GSignalHandler::connectSignal(GObject* source,
        const char* signalName, GCallback callback)
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
        
/*
 * Subscribe a signal handler to receive notifications when a specific 
 * object property changes.
 */
void GSignalHandler::connectNotifySignal
(GObject* source, const char* propertyName)
{   
    if(propertyName != nullptr)
    {
        juce::String signal("notify::");
        signal += propertyName;
        connectSignal(source, signal.toRawUTF8(), 
                G_CALLBACK(notifyCallback));
    }
}

/*
 * Connect to all relevant signals for all GObject signal sources tracked
 * by another signal handler.
 */
void GSignalHandler::shareSignalSources(const GSignalHandler& otherHandler)
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
    
/*
 * Unsubscribes this signal handler from all signals emitted by a GObject.
 */
void GSignalHandler::disconnectSignals(GObject* source)
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
    
/*
 * Unsubscribes the signal handler from all signal sources, and removes all
 * held references to signal sources.
 */
void GSignalHandler::unsubscribeAll()
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

/**
 * Callback to handle property change signals.  This passes all received
 * signals to the signal handler's propertyChanged method.
 */
void GSignalHandler::notifyCallback
(GObject* objectData, GParamSpec* pSpec, GSignalHandler* signalHandler)
{
    GPPObjectType tempObjectHolder(objectData);
    juce::String property(pSpec->name);
    signalHandler->propertyChanged(tempObjectHolder, property);
}