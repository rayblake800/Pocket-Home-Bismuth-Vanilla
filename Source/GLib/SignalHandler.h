#pragma once
#include <glib-object.h>
#include <map>
#include "GLib/WeakRef.h"
#include "JuceHeader.h"

/**
 * @file  GLib/SignalHandler.h
 * 
 * @brief Responds to signals emitted by GLib objects.
 */
class GLib::SignalHandler
{      
public:
    SignalHandler() { }

    /**
     * @brief  Unsubscribes the signal handler from all signal sources, and 
     *         removes all held references to signal sources.
     */
    virtual ~SignalHandler();
    
    /**
     * @brief  Subscribe to all relevant signals from a single GObject signal 
     *         source.
     * 
     * @param source  A GObject this signal handler should track.
     */
    virtual void connectAllSignals(GObject* source) = 0;
    
    /**
     * @brief  Unsubscribes this signal handler from all signals emitted by a 
     *         GObject.
     * 
     * @param source  A GObject signal source.
     *
     * @return        True if the signal source was removed, false if the signal
     *                handler was not subscribed to this object's signals.
     */
    bool disconnectSignals(GObject* source);
    
    /**
     * @brief  Checks if this signal handler is connected to a particular 
     *         GObject signal source.
     *
     * @param source  A GObject to search for in the list of signal sources
     *                handled by this signal handler.
     *
     * @return        Whether this signal handler is connected to the GObject.
     */
    bool isConnected(GObject* source) const;
    
protected:
    /**
     * @brief  Subscribes the signal handler to a single signal.
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
    void connectSignal
    (GObject* source, const char* signalName,  GCallback callback);
        
    /**
     * @brief  Subscribes the signal handler to receive notifications when a 
     *         specific object property changes.
     * 
     * @param source        A GOject signal source.  The signal handler will
     *                      acquire a reference to the source's GObject, if it 
     *                      doesn't have one already.
     * 
     * @param propertyName  A valid property of this object.
     */
    void connectNotifySignal(GObject* source, const char* propertyName);

    /**
     * @brief  Connects to all relevant signals for all GObject signal sources 
     *         tracked by another signal handler.
     * 
     * @param otherHandler  Another signal handler.
     */
    void shareSignalSources(const SignalHandler& otherHandler);

        
    /**
     * @brief  A callback function for handling property change notification 
     *         signals.
     *
     * Signal handlers should override this to handle all specific property 
     * change notifications that they support.
     * 
     * @param source    Holds the GObject that emitted the signal.
     * 
     * @param property  The name of the object property that changed.
     */
    virtual void propertyChanged(GObject* source, juce::String property) { }
    
    /**
     * @brief  Unsubscribes the signal handler from all signal sources, and 
     *         removes all held references to signal sources.
     */
    void unsubscribeAll();
    
private:
    /**
     * @brief  A callback function used to handle property change signals. This 
     *         passes all received signals to the signal handler's 
     *         propertyChanged method.
     * 
     * @param signalSource   The GObject notifying that one of its properties
     *                       has changed.
     * 
     * @param pSpec          Holds the name of the updated property.
     * 
     * @param signalHandler  The signal handler subscribed to the source's 
     *                       property updates. 
     */
    static void notifyCallback
    (GObject* signalSource, GParamSpec* pSpec, SignalHandler* signalHandler);
    
    /* Maps each GObject* signal source to the list of subscribed signal IDs 
       that belong to this SignalHandler.  The SignalHandler holds a reference 
       to each GObject* here, ensuring that all of them remain valid.  */
    juce::HashMap<WeakRef, juce::Array<gulong>,
            juce::DefaultHashFunctions, juce::CriticalSection> signals;
};
