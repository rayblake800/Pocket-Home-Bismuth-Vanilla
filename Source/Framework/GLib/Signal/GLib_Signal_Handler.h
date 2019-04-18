#pragma once
/**
 * @file  GLib_Signal_Handler.h
 *
 * @brief  Responds to signals emitted by GLib objects.
 */

#include "GLib_Signal_Connection.h"
#include "GLib_ObjectPtr.h"
#include "JuceHeader.h"
#include <glib-object.h>

/**
 * @brief  Handles signals emitted by a specific GLib object type.
 *
 * @tparam SourceType  That GLib::Object subclass that holds the type of
 *                     GObject signal source that this signal handler will use.
 *
 *  GLib objects create signals to notify relevant objects that some event has
 * occurred. By default, signals are created by GObjects every time one of
 * their properties changes, specifying exactly which property has changed.
 * GObject classes may provide new signal types to signal whatever events are
 * relevant to them.
 *
 *  Signal::Handler objects connect to GObject signal sources held within
 * GLib::Object subclass instances. They define exactly which signals are
 * relevant to them. Once connected, the signal source will call a static
 * callback function provided by the signal handler every time it emits the
 * connected signal type. Using a CallbackData object provided by the signal
 * handler on connection, that static callback function accesses the connected
 * signal handler, and calls an appropriate function to handle the signal event.
 */
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
     * @return        True if the signal source was removed, false if the
     *                signal handler was not subscribed to this object's
     *                signals.
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
     * @return        True if the signal source was removed, false if the
     *                signal handler was not subscribed to this object's
     *                signals.
     */
    bool disconnectSignals(GObject* source)
    {
        if (source == nullptr)
        {
            return false;
        }
        bool disconnected = false;
        for (int i = 0; i < connections.size(); i++)
        {
            Connection<SourceType>* connection = connections[i];
            if (connection != nullptr && connection->isConnected())
            {
                ObjectPtr connectionSource(connection->getConstSignalSource());
                if (connectionSource == source)
                {
                    disconnected = true;
                }
                else if (connectionSource != nullptr)
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
        if (source == nullptr)
        {
            return false;
        }
        for (const Connection<SourceType>* connection : connections)
        {
            if (connection != nullptr && connection->getConstSignalSource()
                    == source)
            {
                return true;
            }
        }
        return false;
    }

protected:
    /**
     * @brief  Creates a connection to a signal source in order to receive a
     *         specific signal type.
     *
     * @param signal         The name of the signal this Handler should receive.
     *
     * @param callback       A static callback function that will be called
     *                       when the signal is emitted.
     *
     * @param callbackData   Callback data created using the signal source and
     *                       signal handler, to be passed to the callback
     *                       function when the signal is emitted.
     */
    void createConnection(const juce::String signal, GCallback callback,
            CallbackData<SourceType>* callbackData)
    {
        connections.add(new Connection<SourceType>(signal, callback,
                    callbackData));
    }

    /**
     * @brief  Creates a connection to a signal source in order to receive
     *         property update signals when a specific object property changes.
     *
     * @param propertyName  The signal source object property the signal
     *                      handler should track.
     *
     * @param signalSource  The signal source to connect.
     */
    void createPropertyConnection(const juce::String propertyName,
            const SourceType signalSource)
    {
        ObjectPtr sourceObject(signalSource);
        if (sourceObject != nullptr)
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
     *  Signal handlers should override this to handle all specific property
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
        for (int i = 0; i < connections.size(); i++)
        {
            Connection<SourceType>* connection = connections[i];
            if (connection == nullptr)
            {
                connections.remove(i);
                i--;
            }
            else
            {
                ObjectPtr sourcePtr(connection->getSignalSource());
                if (sourcePtr == nullptr)
                {
                    connections.remove(i);
                    i--;
                }
            }
        }
    }

    /**
     * @brief  A callback function used to handle property change signals. This
     *         passes all received signals to the signal handler's
     *         propertyChanged function.
     *
     * @param signalSource   The GObject notifying that one of its properties
     *                       has changed.
     *
     * @param pSpec          Holds the name of the updated property.
     *
     * @param data           The callback data structure used to handle the
     *                       property update.
     */
    static void notifyCallback(GObject* signalSource, GParamSpec* pSpec,
            CallbackData<SourceType>* data);

    // Tracks all connections to signal sources:
    juce::OwnedArray<Connection<SourceType>> connections;
};

/**
 * @brief  A callback function used to handle property change signals. This
 *         passes all received signals to the signal handler's propertyChanged
 *         function.
 *
 * @param signalSource   The GObject notifying that one of its properties has
 *                       changed.
 *
 * @param pSpec          Holds the name of the updated property.
 *
 * @param data           The callback data structure used to handle the property
 *                       update.
 */
template <class SourceType>
void GLib::Signal::Handler<SourceType>::notifyCallback(GObject* signalSource,
        GParamSpec* pSpec, CallbackData<SourceType>* data)
{
    if (signalSource != nullptr && data != nullptr && pSpec != nullptr)
    {
        SourceType source = data->getSignalSource();
        Handler<SourceType>* handler = data->getSignalHandler();
        if (!source.isNull() && handler != nullptr)
        {
            handler->propertyChanged(source, pSpec->name);
        }
    }
}
