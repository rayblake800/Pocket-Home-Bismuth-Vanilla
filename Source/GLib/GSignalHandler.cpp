#include "GSignalHandler.h"

/*
 * Unsubscribes the signal handler from all signal sources, and removes all
 * held references to signal sources.
 */
GSignalHandler::~GSignalHandler()
{
    using namespace juce;
    unsubscribeAll();
}
    
/*
 * Unsubscribes this signal handler from all signals emitted by a GObject.
 */
bool GSignalHandler::disconnectSignals(GObject* source)
{
    using namespace juce;
    if(source != nullptr)
    {
        const ScopedLock changeSourceLock(signals.getLock());
        GPPWeakRef sourceRef(source);
        if(signals.contains(sourceRef))
        {
            for(const guint& signalID : signals[sourceRef])
            {
                g_signal_handler_disconnect(source, signalID);
            }
            signals.remove(sourceRef);
            g_object_unref(source);
            return true;
        }
    }
    return false;
}

/*
 * Checks if this signal handler is connected to a particular GObject signal
 * source.
 */
bool GSignalHandler::isConnected(GObject* source) const
{
    using namespace juce;
    if(source == nullptr)
    {
        return false;
    }
    const ScopedLock signalLock(signals.getLock());
    GPPWeakRef sourceRef(source);
    return signals.contains(sourceRef);
}

/*
 * Subscribe the signal handler to a single signal.
 */
void GSignalHandler::connectSignal(GObject* source,
        const char* signalName, GCallback callback)
{
    using namespace juce;
    if(source == nullptr || signalName == nullptr)
    {
        return;
    }
    const juce::ScopedLock signalLock(signals.getLock());
    gulong handlerID = g_signal_connect
            (source, signalName, callback, this);
    if(handlerID != 0)
    {
        GPPWeakRef sourceRef(source);
        
        if(!signals.contains(sourceRef))
        {
            g_object_ref(source);
        }
        signals.getReference(sourceRef).add(handlerID);
    }       
    else
    {
        DBG("GSignalHandler::" << __func__ << ": Failed to subscribe to \""
                << signalName << "\" signal.");
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
    const juce::ScopedLock readSourceLock(otherHandler.signals.getLock());
    auto iter = otherHandler.signals.begin();
    while(iter.next())
    {
        GObject* source = iter.getKey().getObject();
        if(source != nullptr)
        {
            signalSources.add(source);
        }
    }
    const juce::ScopedUnlock readFinished(otherHandler.signals.getLock());

    for(GObject* source : signalSources)
    {
        connectAllSignals(source);
        g_object_unref(source);
    }
    
}
    
/*
 * Unsubscribes the signal handler from all signal sources, and removes all
 * held references to signal sources.
 */
void GSignalHandler::unsubscribeAll()
{
    using namespace juce;
    const ScopedLock changeSourceLock(signals.getLock());
    for(auto iter = signals.begin(); iter != signals.end(); iter.next())
    {
        GObject* source = iter.getKey().getObject();
        int count = source->ref_count;
        jassert(source->ref_count > 1);
        if(source != nullptr)
        {
            for(const guint& signalID : iter.getValue())
            {
                g_signal_handler_disconnect(source, signalID);
            }
            g_object_unref(source);
        }
        g_clear_object(&source);
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
    juce::String property(pSpec->name);
    signalHandler->propertyChanged(objectData, property);
}
