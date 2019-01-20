#include "GLib_SignalHandler.h"
#include "GLib_Object.h"


/*
 * Unsubscribes the signal handler from all signal sources, and removes all
 * held references to signal sources.
 */
GLib::SignalHandler::~SignalHandler()
{
    unsubscribeAll();
}

/*
 * Subscribe to all relevant signals from a single GObject signal source.
 */
void GLib::SignalHandler::connectAllSignals(Object& source)
{
    ObjectPtr sourcePtr(source);
    if(sourcePtr != nullptr && !isConnected(sourcePtr))
    {
        addSignalSourceIfNew(sourcePtr, source.isOwned());
        connectAllSignals(sourcePtr);
    }
}

/*
 * Unsubscribes this signal handler from all signals emitted by a GObject.
 */
bool GLib::SignalHandler::disconnectSignals(Object& source)
{
    return disconnectSignals(ObjectPtr(source));
}

/*
 * Unsubscribes this signal handler from all signals emitted by a GObject.
 */
bool GLib::SignalHandler::disconnectSignals(GObject* source)
{
    if(source != nullptr && isConnected(source))
    {
        SourceData sourceData = signals[source];
        for(const guint& signalID : sourceData.signalIDs)
        {
            g_signal_handler_disconnect(source, signalID);
        }
        const juce::ScopedLock signalListLock(signals.getLock());
        if(sourceData.referenceHeld)
        {
            g_object_unref(source);
        }
        signals.remove(source);
        return true;
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
    const juce::ScopedLock signalListLock(signals.getLock());
    return signals.contains(source);
}

/*
 * Subscribes the signal handler to a single signal.
 */
void GLib::SignalHandler::connectSignal
(Object& source, const char* signalName, GCallback callback)
{
    ObjectPtr sourcePtr(source);
    if(sourcePtr != nullptr)
    {
        connectSignal(sourcePtr, signalName, callback, source.isOwned());
    }
}

/*
 * Subscribes the signal handler to a single signal.
 */
void GLib::SignalHandler::connectSignal(
        GObject* source,
        const char* signalName,
        GCallback callback,
        const bool shouldHaveRef)
{
    jassert(signalName != nullptr);
    if(source != nullptr)
    {
        gulong handlerID = g_signal_connect(source, signalName, callback, this);
        if(handlerID != 0)
        {
            addSignalSourceIfNew(source, shouldHaveRef);
            const juce::ScopedLock signalLock(signals.getLock());
            signals.getReference(source).signalIDs.add(handlerID);
        }
        else
        {
            DBG("GLib::SignalHandler::" << __func__ 
                    << ": Failed to subscribe to \"" << signalName
                    << "\" signal.");
        }
    }
}

/*
 * Subscribes the signal handler to receive notifications when a specific
 * object property changes.
 */
void GLib::SignalHandler::connectNotifySignal
(Object& source, const char* propertyName)
{
    ObjectPtr sourcePtr(source);
    if(source != nullptr)
    {
        connectNotifySignal(sourcePtr, propertyName, source.isOwned());
    }
}

/*
 * Subscribes the signal handler to receive notifications when a specific object
 * property changes.
 */
void GLib::SignalHandler::connectNotifySignal(GObject* source, 
        const char* propertyName, const bool shouldHaveRef)
{
    if(propertyName != nullptr)
    {
        juce::String signal("notify::");
        signal += propertyName;
        connectSignal(source, signal.toRawUTF8(), G_CALLBACK(notifyCallback),
                shouldHaveRef);
    }
}

/*
 * Connects to all relevant signals for all GObject signal sources tracked by
 * another signal handler.
 */
void GLib::SignalHandler::shareSignalSources(const SignalHandler& otherHandler)
{
    struct SourceInitData
    {
        WeakRef source;
        bool shouldHaveRef;
    };
    juce::Array<SourceInitData> sharedSources;

    const juce::ScopedLock readSourceLock(otherHandler.signals.getLock());
    SignalHashMap::Iterator iter(otherHandler.signals);
    while(iter.next())
    {
        sharedSources.add({iter.getKey(), iter.getValue().referenceHeld});
    }
    const juce::ScopedUnlock readFinishedUnlock(otherHandler.signals.getLock());

    for(SourceInitData& sourceData : sharedSources)
    {
        ObjectPtr sourcePtr(sourceData.source.getObject());
        if(sourcePtr != nullptr && !isConnected(sourcePtr))
        {
            addSignalSourceIfNew(sourcePtr, sourceData.shouldHaveRef);
            connectAllSignals(sourcePtr);
        }
    }
}

/*
 * Unsubscribes the signal handler from all signal sources, and removes any
 * held references to signal sources.
 */
void GLib::SignalHandler::unsubscribeAll()
{
    const juce::ScopedLock clearSignalLock(signals.getLock());
    SignalHashMap::Iterator iter(signals);
    while(iter.next())
    {
        ObjectPtr sourcePtr(iter.getKey().getObject());
        if(sourcePtr != nullptr)
        {
            SourceData sourceData = iter.getValue();
            for(const guint& signalID : sourceData.signalIDs)
            {
                g_signal_handler_disconnect(sourcePtr, signalID);
            }
            if(sourceData.referenceHeld)
            {
                g_object_unref(sourcePtr);
            }
        }
    }
    signals.clear();
}

/*
 * A callback function used to handle property change signals. This passes all
 * received signals to the signal handler's propertyChanged method.
 */
void GLib::SignalHandler::notifyCallback
(GObject* signalSource, GParamSpec* pSpec, SignalHandler* signalHandler)
{
    juce::String property(pSpec->name);
    signalHandler->propertyChanged(signalSource, property);
}


/*
 * Ensures a signal source exists in the signal map, and that a reference to it
 * is held if necessary.
 */
void GLib::SignalHandler::addSignalSourceIfNew
(GObject* source, const bool shouldHaveRef)
{
    const juce::ScopedLock clearSignalLock(signals.getLock());
    if(!signals.contains(source))
    {
        signals.set(WeakRef(source), { .referenceHeld = shouldHaveRef });
        if(shouldHaveRef)
        {
            g_object_ref(source);
        }
    }
}
