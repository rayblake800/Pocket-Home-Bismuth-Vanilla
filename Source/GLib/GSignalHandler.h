#pragma once
#include <glib.h>
#include <map>
#include "JuceHeader.h"
/**
 * @file GSignalHandler.h
 * 
 * @brief Responds to signals emitted by GLib objects.
 */
class GSignalHandler
{      
public:
    GSignalHandler() { }

    /**
     * Unsubscribes the signal handler from all signal sources, and removes all
     * held references to signal sources.
     */
    virtual ~GSignalHandler();
    
    /**
     * Subscribe to all relevant signals from a single GObject signal source.
     * 
     * @param source  A GObject this signal handler should track.
     */
    virtual void connectAllSignals(GObject* source) = 0;
    
protected:
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
    void connectSignal
    (GObject* source, const char* signalName,  GCallback callback);
        
    /**
     * Subscribe a signal handler to receive notifications when a specific 
     * object property changes.
     * 
     * @param source      A GOject signal source.  The signal handler will
     *                    acquire a reference to the source's GObject, if it 
     *                    doesn't have one already.
     * 
     * @param propertyName  A valid property of this object.
     */
    void connectNotifySignal(GObject* source, const char* propertyName);

    /**
     * Connect to all relevant signals for all GObject signal sources tracked
     * by another signal handler.
     * 
     * @param otherHandler  Another signal handler.
     */
    void shareSignalSources(const GSignalHandler& otherHandler);

        
    /**
     * Callback function for handling property change notification signals.
     * Signal handlers should override this to handle all specific property 
     * change notifications that they support.
     * 
     * @param source    Holds the GObject that emitted the signal.
     * 
     * @param property  The name of the object property that changed.
     */
    virtual void propertyChanged(GObject* source, juce::String property) { }
    
    /**
     * Unsubscribes this signal handler from all signals emitted by a GObject.
     * 
     * @param source  A GObject signal source.
     */
    void disconnectSignals(GObject* source);
    
    /**
     * Unsubscribes the signal handler from all signal sources, and removes all
     * held references to signal sources.
     */
    void unsubscribeAll();
    
private:
    /**
     * Callback to handle property change signals.  This passes all received
     * signals to the signal handler's propertyChanged method.
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
    (GObject* signalSource, GParamSpec* pSpec, GSignalHandler* signalHandler);
    
    /**
     * Maps each GObject* signal source to the list of subscribed signal IDs 
     * that belong to this SignalHandler.  The SignalHandler holds a reference 
     * to each GObject* here, ensuring that all of them remain valid.
     */
    juce::HashMap<GObject*, juce::Array<gulong>,
            juce::DefaultHashFunctions, juce::CriticalSection> signals;
};
