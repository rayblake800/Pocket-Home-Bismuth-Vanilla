#include "GLib/SignalHandler.h"

/*
 * Unsubscribes the signal handler from all signal sources, and removes all
 * held references to signal sources.
 */
GLib::SignalHandler::~SignalHandler()
{
    using namespace juce;
    unsubscribeAll();
}
    
/*
 * Unsubscribes this signal handler from all signals emitted by a GObject.
 */
bool GLib::SignalHandler::disconnectSignals(GObject* source)
{
    if(source != nullptr)
    {
        const juce::ScopedLock changeSourceLock(signals.getLock());
        WeakRef sourceRef(source);
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
bool GLib::SignalHandler::isConnected(GObject* source) const
{
    if(source == nullptr)
    {
        return false;
    }
    const juce::ScopedLock signalLock(signals.getLock());
    WeakRef sourceRef(source);
    return signals.contains(sourceRef);
}

/*
 * Subscribes the signal handler to a single signal.
 */
void GLib::SignalHandler::connectSignal(GObject* source,
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
        WeakRef sourceRef(source);
        
        if(!signals.contains(sourceRef))
        {
            g_object_ref(source);
        }
        signals.getReference(sourceRef).add(handlerID);
    }       
    else
    {
        DBG("GLib::SignalHandler::" << __func__ << ": Failed to subscribe to \""
                << signalName << "\" signal.");
    }
}
        
/*
 * Subscribes the signal handler to receive notifications when a specific 
 * object property changes.
 */
void GLib::SignalHandler::connectNotifySignal
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
 * Connects to all relevant signals for all GObject signal sources tracked
 * by another signal handler.
 */
void GLib::SignalHandler::shareSignalSources(const SignalHandler& otherHandler)
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
void GLib::SignalHandler::unsubscribeAll()
{
    const juce::ScopedLock changeSourceLock(signals.getLock());
    for(auto iter = signals.begin(); iter != signals.end(); iter.next())
    {
        GObject* source = iter.getKey().getObject();
        if(source != nullptr)
        {
            int count = source->ref_count;
            jassert(source->ref_count > 1);
            for(const guint& signalID : iter.getValue())
            {
                g_signal_handler_disconnect(source, signalID);
            }
            g_object_unref(source);
            g_clear_object(&source);
        }
    }
    signals.clear();
}

/*
 * Callback to handle property change signals.  This passes all received
 * signals to the signal handler's propertyChanged method.
 */
void GLib::SignalHandler::notifyCallback
(GObject* objectData, GParamSpec* pSpec, SignalHandler* signalHandler)
{
    juce::String property(pSpec->name);
    signalHandler->propertyChanged(objectData, property);
}
