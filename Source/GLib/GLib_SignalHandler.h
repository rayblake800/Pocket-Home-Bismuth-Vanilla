#pragma once
/**
 * @file  GLib_SignalHandler.h
 * 
 * @brief Responds to signals emitted by GLib objects.
 */

#include "GLib_WeakRef.h"
#include "JuceHeader.h"
#include <glib-object.h>

namespace GLib { class SignalHandler; }
namespace GLib { class Object; }

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
     * @param source  A GObject* this signal handler should track.
     */
    void connectAllSignals(Object& source);
    
    /**
     * @brief  Unsubscribes this signal handler from all signals emitted by a 
     *         GObject.
     * 
     * @param source  A GLib::Object signal source.
     *
     * @return        True if the signal source was removed, false if the signal
     *                handler was not subscribed to this object's signals.
     */
    bool disconnectSignals(Object& source);
    
    /**
     * @brief  Unsubscribes this signal handler from all signals emitted by a 
     *         GObject.
     * 
     * @param source  A GObject* signal source.
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
     * @brief  Subscribe to all relevant signals from a single GObject signal 
     *         source.
     *
     * SignalHandler subclasses must implement this to track the specific set
     * of signals that they need to receive.
     * 
     * @param source  A GObject* this signal handler should track.
     */
    virtual void connectAllSignals(GObject* source) = 0;

    /**
     * @brief  Subscribes the signal handler to a single signal.
     * 
     * @param source      A GLib::Object signal source. If this source is Owned,
     *                    the signal handler will acquire a reference to its 
     *                    GObject if it doesn't have one already.
     * 
     * @param signalName  The signal that handler should receive.
     * 
     * @param callback    A signal-specific callback function.  This should
     *                    be a static void function that takes parameters 
     *                    specific to the subscribed signal.  A pointer to 
     *                    this signal handler will be passed to the callback
     *                    as data.
     */
    void connectSignal(Object& source, const char* signalName, 
            GCallback callback);

    /**
     * @brief  Subscribes the signal handler to a single signal.
     * 
     * @param source         A GObject* signal source.
     * 
     * @param signalName     The signal that handler should receive.
     * 
     * @param callback       A signal-specific callback function.  This should
     *                       be a static void function that takes parameters 
     *                       specific to the subscribed signal.  A pointer to 
     *                       this signal handler will be passed to the callback
     *                       as data.
     *
     * @param shouldHaveRef  Whether the SignalHandler should hold a reference
     *                       to the source.
     */
    void connectSignal(GObject* source, const char* signalName, 
            GCallback callback, const bool shouldHaveRef);
        
    /**
     * @brief  Subscribes the signal handler to receive notifications when a 
     *         specific object property changes.
     * 
     * @param source        A GLib::Object signal source. If this source is 
     *                      Owned, the signal handler will acquire a reference 
     *                      to its GObject if it doesn't have one already.
     * 
     * @param propertyName  A valid property of this object.
     */
    void connectNotifySignal(Object& source, const char* propertyName);

    /**
     * @brief  Subscribes the signal handler to receive notifications when a 
     *         specific object property changes.
     * 
     * @param source         A GObject* signal source. 
     * 
     * @param propertyName   A valid property of this object.
     *
     * @param shouldHaveRef  Whether the SignalHandler should hold a reference
     *                       to the source.
     */
    void connectNotifySignal(GObject* source, const char* propertyName,
            const bool shouldHaveRef);

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
     *         removes any held references to signal sources.
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
    
    /**
     * @brief  Holds data about a single signal source.
     */
    struct SourceData
    {
        /* Tracks if this signal source is an Owned object with a reference held
           by the SignalHandler. */
        bool referenceHeld;
        /* Lists the signal IDs of all tracked signals. */
        juce::Array<gulong> signalIDs;
    };

    /**
     * @brief  Ensures a signal source exists in the signal map, and that
     *         a reference to it is held if necessary.
     *
     * @param source         A GObject* signal source that was or will be added.
     *
     * @param shouldHaveRef  Whether the SignalHandler should hold a reference
     *                       to that source.
     */
    void addSignalSourceIfNew(GObject* source, const bool shouldHaveRef);
    
    /* The specific HashMap type used to store SourceData. */
    typedef juce::HashMap<WeakRef, SourceData, juce::DefaultHashFunctions,
            juce::CriticalSection> SignalHashMap;

    /* Maps each GObject* signal source to its signal SourceData. */
    SignalHashMap signals;
};
