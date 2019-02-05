#pragma once
/**
 * @file  GLib_Signal_CallbackData.h
 *
 * @brief  Holds extra data used when handling a GLib object signal.
 */

namespace GLib
{
    namespace Signal
    {
        template <class SourceType> class CallbackData;
        template <class SourceType> class Handler;
    }
}

#include <glib-object.h>

/**
 * @tparam SourceType  The GLib::Object subclass of the signal source. 
 */
template <class SourceType> 
class GLib::Signal::CallbackData
{
public:
    /**
     * @brief  Create a CallbackData object for a single connection between a
     *         signal source and its signal handler.
     *
     * @param source   The GLib::Object signal source.
     *
     * @param handler  The object handling signals from that source.
     */
    CallbackData(SourceType& source, Handler<SourceType>* handler) :
        signalSource(source), signalHandler(handler) { }

    virtual ~CallbackData() { }

    /**
     * @brief  Gets the signal source object.
     *
     * @return  The source object, or a null object if the source is no longer
     *          valid.
     */
    SourceType& getSignalSource()
    {
        return signalSource;
    }

    /**
     * @brief  Gets a pointer to the signal handler object.
     *
     * @return  The signal handler pointer. 
     */
    Handler<SourceType>* getSignalHandler() const
    {
        return signalHandler;
    }

private:
    SourceType signalSource;
    Handler<SourceType>* signalHandler;
};
