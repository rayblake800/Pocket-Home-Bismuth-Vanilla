#include "GLibEventHandler.h"

GLibEventHandler::GLibEventHandler() : Thread("GLibEventHandler")
{
    context = g_main_context_new();
    mainLoop = g_main_loop_new(context, false);
    startThread();
}

GLibEventHandler::~GLibEventHandler()
{
    g_main_loop_quit(mainLoop);
    signalThreadShouldExit();
    waitForThreadToExit(-1);
    g_main_loop_unref(mainLoop);
    g_main_context_unref(context);
    mainLoop = nullptr;
    context = nullptr;
}

/**
 * Asynchronously run a function once on the GLib event loop.
 */
void GLibEventHandler::gLibCallAsync(std::function<void() > fn)
{
    addAndInitCall(fn);
}

/**
 * Run a function on the GLib event loop, yielding until the function
 * has finished.
 */
void GLibEventHandler::gLibCall(std::function<void() > fn)
{
    GSource* trackedSource = addAndInitCall(fn);
    //this should never be called from within the event thread!
    jassert(Thread::getCurrentThreadId != getThreadId());
    while (gSourceTracker.contains(trackedSource))
    {
        Thread::yield();
    }
}

/**
 * Adds a function to the GMainContext so it will execute on the event
 * thread.
 */
GSource* GLibEventHandler::addAndInitCall
(std::function<void() > call)
{
    CallData* callData = new CallData;
    callData->call = call;
    callData->callSource = g_idle_source_new();
    callData->sourceTracker = &gSourceTracker;
    g_source_set_callback(
            callData->callSource,
            (GSourceFunc) runAsync,
            callData,
            nullptr);
    g_source_attach(callData->callSource, context);
    return callData->callSource;
}

/**
 * Runs the GLib main loop.
 */
void GLibEventHandler::run()
{
    if (context != nullptr)
    {
        g_main_context_push_thread_default(context);
        g_main_loop_run(mainLoop);
    }
}

/**
 * Callback function used to execute arbitrary functions on the 
 * GMainLoop.
 */
gboolean GLibEventHandler::runAsync
(GLibEventHandler::CallData* runData)
{
    g_assert(g_main_context_is_owner(g_main_context_get_thread_default()));
    runData->call();
    runData->sourceTracker->removeAllInstancesOf(runData->callSource);
    g_source_destroy(runData->callSource);
    delete runData;
    return true;
}