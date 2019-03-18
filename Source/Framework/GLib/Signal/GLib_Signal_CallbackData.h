#pragma once
/**
 * @file  GLib_Signal_CallbackData.h
 *
 * @brief  Holds extra data used when handling a GLib object signal.
 */

#include <glib-object.h>

namespace GLib
{
    namespace Signal
    {
        template <class SourceType> class CallbackData;
        template <class SourceType> class Handler;
    }
}


/**
 * @brief  Saves a copy of the signal source object and a signal handler pointer
 *         to use as callback data when handling GLib object signals.
 *
 * @tparam SourceType  The GLib::Object subclass of the signal source. 
 *
 *  Whenever a GObject emits a signal, if a Signal::Handler is connected for
 * that signal type and signal source, a CallbackData object is passed in as an
 * argument to a static callback function provided by the signal hander class.
 * The CallbackData object contains the address of the signal handler, and a 
 * copy of the GLib::Object containing the GObject signal source. The static 
 * callback function should use the CallbackData to call an appropriate signal 
 * handling method on the signal handler, optionally passing in the GLib::Object
 * containing the signal source as a parameter.
 *
 *  Signal handlers must create and provide a new CallbackData object for each
 * connection they have with a signal source. Signal handlers may create and
 * use subclasses of CallbackData to provide additional signal handling data.
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
    CallbackData(const SourceType source, Handler<SourceType>* handler) :
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
    /* A copy of the GLib::Object signal source container: */
    SourceType signalSource;
    /* A pointer to the Signal::Handler that created this CallbackData: */
    Handler<SourceType>* signalHandler;
};
