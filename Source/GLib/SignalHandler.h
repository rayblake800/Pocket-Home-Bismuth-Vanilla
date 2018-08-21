#pragma once
#include <glib.h>
#include <map>
#include "JuceHeader.h"
/**
 * @file SignalHandler.h
 * 
 * @brief Responds to signals emitted by GLib objects.
 */

template<class GPPObjectType> class GPPObject;

template <class GPPObjectType>
class SignalHandler
{    
protected:
    SignalHandler() { }

    /**
     * Copies an existing signal handler, subscribing to all of that
     * handler's tracked signals.
     * 
     * @param rhs  Another valid signal handler.
     */
    SignalHandler(const SignalHandler<GPPObjectType>& rhs);

public:
    /**
     * Unsubscribes the signal handler from all signal sources, and removes all
     * held references to signal sources.
     */
    virtual ~SignalHandler();
    
protected:
    /**
     * Subscribes this signal handler to all relevant signals emitted by 
     * a GPPObject.
     * 
     * @param source  A GPPObject signal source.
     */
    void subscribe(GPPObjectType& source);
    
    /**
     * Unsubscribes this signal handler from all signals emitted by a GPPObject.
     * 
     * @param source  A GPPObject signal source.
     */
    void unsubscribe(GPPObjectType& source);
    
    /**
     * Unsubscribes the signal handler from all signal sources, and removes all
     * held references to signal sources.
     */
    void unsubscribeAll();
    
private:
    //Only GPPObjects of the correct type may add themselves as signal sources.
    friend class GPPObject<GPPObjectType>;
          
    /**
     * Subscribe the signal handler from a single signal and signal source.
     * 
     * @param source      A GPPOject signal source.  The signal handler will
     *                    acquire a reference to the source's GObject, if it 
     *                    doesn't have one already.
     * 
     * @param signalName  The signal that handler should receive.
     * 
     * @param callback    A signal-specific callback function.  This should be a
     *                    static void function that takes parameters specific
     *                    to the subscribed signal.  A pointer to this signal
     *                    handler will be passed to the callback as data.
     */
    void connectSignalHandler(GPPObjectType& source,
            const char* signalName, GCallback callback);
    
    /**
     * Maps each GObject* signal source to the list of subscribed signal IDs 
     * that belong to this SignalHandler.  The SignalHandler holds a reference 
     * to each GObject* here, ensuring that all of them remain valid.
     */
    juce::HashMap<GObject*, juce::Array<gulong>,
            juce::DefaultHashFunctions, juce::CriticalSection> signals;
};
