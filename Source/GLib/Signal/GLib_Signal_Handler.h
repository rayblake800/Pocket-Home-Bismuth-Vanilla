#pragma once

/**
 * @file  GLib_Signal_Handler.h
 * 
 * @brief Responds to signals emitted by GLib objects.
 */

#include "GLib_Signal_Connection.h"
#include "GLib_ObjectPtr.h"
#include "JuceHeader.h"
#include <glib-object.h>

template <class SourceType>
class GLib::Signal::Handler
{      
public:
    Handler() { }

    /**
     * @brief  Unsubscribes the signal handler from all signal sources.
     */
    virtual ~Handler()
    {
        connections.clear();
    }
    
    /**
     * @brief  Subscribes to all relevant signals from a single signal source.
     * 
     * @param source  A GLib::Object this signal handler should track.
     */
    virtual void connectAllSignals(const SourceType source) = 0;
    
    /**
     * @brief  Unsubscribes this signal handler from all signals emitted by a 
     *         signal Source.
     * 
     * @param source  A GLib::Object signal source.
     *
     * @return        True if the signal source was removed, false if the signal
     *                handler was not subscribed to this object's signals.
     */
    bool disconnectSignals(const SourceType source)
    {
        ObjectPtr sourcePtr(source);
        return disconnectSignals(sourcePtr);
    }
    
    /**
     * @brief  Unsubscribes this signal handler from all signals emitted by a 
     *         signal Source.
     * 
     * @param source  A GObject* signal source.
     *
     * @return        True if the signal source was removed, false if the signal
     *                handler was not subscribed to this object's signals.
     */
    bool disconnectSignals(GObject* source)
    {
        if(source == nullptr)
        {
            return false;
        }
        bool disconnected = false;
        for(int i = 0; i < connections.size(); i++)
        {
            Connection<SourceType>* connection = connections[i];
            if(connection != nullptr && connection->isConnected())
            {
                ObjectPtr connectionSource(connection->getConstSignalSource());
                if(connectionSource == source)
                {
                    disconnected = true;
                }
                else if(connectionSource != nullptr)
                {
                    // Valid, non-matching connection, do not remove.
                    continue;
                }
            }
            connections.remove(i);
            i--;
        }
        return disconnected;
    }
    
    /**
     * @brief  Checks if this signal handler is connected to a particular 
     *         GLib::Object signal source.
     *
     * @param source  An object to search for in the list of signal sources
     *                handled by this signal handler.
     *
     * @return        Whether this signal handler is connected to the GObject.
     */
    bool isConnected(const SourceType source) const
    {
        ObjectPtr sourcePtr(source);
        return isConnected(sourcePtr);
    }
    
    /**
     * @brief  Checks if this signal handler is connected to a particular 
     *         GObject* signal source.
     *
     * @param source  An object to search for in the list of signal sources
     *                handled by this signal handler.
     *
     * @return        Whether this signal handler is connected to the GObject.
     */
    bool isConnected(GObject* source) const
    {
        if(source == nullptr)
        {
            return false;
        }
        for(const Connection<SourceType>* connection : connections)
        {
            if(connection != nullptr && connection->getConstSignalSource() 
                    == source)
            {
                return true;
            }
        }
        return false;
    }

protected:
    /**
     * @brief 
     *
     * @param signal
     * @param callback
     * @param callbackData
     */
    void createConnection(const juce::String signal, GCallback callback,
            CallbackData<SourceType>* callbackData)
    {
        connections.add(new Connection<SourceType>(signal, callback,
                    callbackData));
    }
    
    /**
     * @brief 
     *
     * @param propertyName
     * @param signalSource
     */
    void createPropertyConnection(const juce::String propertyName,
            const SourceType signalSource)
    {
        ObjectPtr sourceObject(signalSource);
        if(sourceObject != nullptr)
        {
            createConnection(juce::String("notify::") + propertyName,
                    G_CALLBACK(notifyCallback),
                    new CallbackData<SourceType>(signalSource, this));
        }
    }

    /**
     * @brief  Disconnects the signal handler from all signal sources, and 
     *         removes any held references to signal sources.
     */
    void disconnectAll()
    {
        connections.clear();
    }

private:
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
    virtual void propertyChanged(const SourceType source,
            juce::String property) { }
    
    /**
     * @brief  Removes all tracked signal sources that are no longer valid.
     */
    void clearInvalidatedSources()
    {
        for(int i = 0; i < connections.size(); i++)
        {
            Connection<SourceType>* connection = connections[i];
            if(connection == nullptr)
            {
                connections.remove(i);
                i--;
            }
            else
            {
                ObjectPtr sourcePtr(connection->getSignalSource());
                if(sourcePtr == nullptr)
                {
                    connections.remove(i);
                    i--;
                }
            }
        }
    }

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
     * @param data           The callback data structure used to handle the
     *                       property update. 
     */
    static void notifyCallback (GObject* signalSource, GParamSpec* pSpec,
            CallbackData<SourceType>* data);

    /* Tracks all connections to signal sources: */
    juce::OwnedArray<Connection<SourceType>> connections;
};

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
 * @param data           The callback data structure used to handle the
 *                       property update. 
 */
template <class SourceType>
void GLib::Signal::Handler<SourceType>::notifyCallback(GObject* signalSource,
        GParamSpec* pSpec, CallbackData<SourceType>* data)
{
    if(signalSource != nullptr && data != nullptr && pSpec != nullptr)
    {
        SourceType source = data->getSignalSource();
        Handler<SourceType>* handler = data->getSignalHandler();
        if(!source.isNull() && handler != nullptr)
        {
            handler->propertyChanged(source, pSpec->name);
        }
    }
}
