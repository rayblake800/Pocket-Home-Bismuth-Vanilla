#pragma once
/**
 * @file  GLib_Signal_Connection.h
 *
 * @brief  Handles a single connection between a GObject* signal source and a
 *         signal handler.
 */

#include "GLib_Signal_CallbackData.h"
#include "GLib_ObjectPtr.h"
#include "JuceHeader.h"

namespace GLib { namespace Signal { 
        template <class SourceType> class Connection; } } 

/**
 * @brief  Represents a connection to a GLib::Object signal source.
 *
 * @tparam SourceType  The GLib::Object subclass of the signal source.
 */
template <class SourceType>
class GLib::Signal::Connection
{
private:
    /* Only the Handler may create connections to signal sources. */
    friend class Handler<SourceType>;

    /**
     * @brief  Creates a new connection to a signal source.
     *
     * @param signal      The name of a signal the source object may create.
     *
     * @param callback    The callback function used to handle that signal 
     *                    type.
     *
     * @param data        The collection of data needed to handle signal
     *                    callbacks.
     */
    Connection(const juce::String signal, GCallback callback, 
            CallbackData<SourceType>* data) 
    {
        if(data != nullptr)
        {
            signalData.reset(data);
            ObjectPtr sourcePtr(data->getSignalSource());
            if(sourcePtr != nullptr)
            {
                connectionID = g_signal_connect(sourcePtr, signal.toRawUTF8(),
                        callback, data);
                if(connectionID != 0)
                {
                    signalName = signal;
                }
            }
        }
    }
public:

    /**
     * @brief  Creates an invalid connection object.
     */
    Connection() { }

    /**
     * @brief  Ensures the connection is disconnected on destruction.
     */
    virtual ~Connection() 
    {
        disconnect();
    }

    /**
     * @brief  Gets the signal name set on construction.
     *
     * @return  The signal name, or the empty string if this object never
     *          attempted to connect to a signal source.
     */
    juce::String getSignalName() const
    {
        return signalName;
    }

    /**
     * @brief  Gets the ID number used to identify this connection.
     *
     * @return  The connection ID number, or 0 if not connected to a signal
     *          source.
     */
    gulong getConnectionID() const
    {
        return connectionID;
    }

    /**
     * @brief  Gets the GLib::Object signal source.
     *
     * @return  The signal source object, or a null object if the signal source
     *          is no longer valid.
     */
    SourceType& getSignalSource()
    {
        if(signalData == nullptr)
        {
            return nullSource;
        }
        return signalData->getSignalSource();
    }

    /**
     * @brief  Gets a constant reference to the GLib::Object signal source.
     *
     * @return  The signal source object, or a null object if the signal source
     *          is no longer valid.
     */
    const SourceType& getConstSignalSource() const
    {
        if(signalData == nullptr)
        {
            return nullSource;
        }
        return signalData->getSignalSource();
    }
    
    /**
     * @brief  Checks if this connection object is valid.
     *
     * @return  Whether the signal source and the signal handler are linked 
     *          through this connection.
     */
    bool isConnected() const
    {
        if(connectionID != 0 && signalData != nullptr)
        {
            ObjectPtr sourcePtr(signalData->getSignalSource());
            return sourcePtr != nullptr;
        }
        return false;
    }

    /**
     * @brief  Closes this connection between the signal handler and the signal
     *         source.
     *
     * If the connection is already invalid, no action will be taken.
     */
    void disconnect()
    {
        ObjectPtr sourcePtr(getSignalSource());
        if(sourcePtr != nullptr && connectionID != 0)
        {
            g_signal_handler_disconnect(sourcePtr, connectionID);
            connectionID = 0;
            signalData.reset(nullptr);
        }
    }

private:
    /* Holds the signal source copy and signal handler pointer, possibly with
     * other relevant callback data: */
    std::unique_ptr<CallbackData<SourceType>> signalData;
    /* The name of the connected signal: */
    juce::String signalName;
    /* An ID that may be used to cancel the connection: */
    gulong connectionID = 0;
    /* Alternate null source to return if the original source is invalid: */
    SourceType nullSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Connection);
};
