#include "GLibSignalHandler.h"


ScopedPointer<SharedResource> GLibSignalHandler::globalThread;
CriticalSection GLibSignalHandler::threadLock;

GLibSignalHandler::GLibSignalHandler()
: ResourceManager(globalThread, threadLock, []()
{

    return new GLibThread();
}) { }

/**
 * Returns true if it's being executed on the GLib event thread.
 */
bool GLibSignalHandler::runningOnGLibThread()
{
    const ScopedLock accessLock(threadLock);
    GLibThread* thread = static_cast<GLibThread*> (globalThread);
    return Thread::getCurrentThreadId == thread->getThreadId();
}

/**
 * Asynchronously run a function once on the GLib event loop.
 */
void GLibSignalHandler::gLibCallAsync(std::function<void() > fn)
{
    const ScopedLock accessLock(threadLock);
    GLibThread* thread = static_cast<GLibThread*> (globalThread);
    thread->addAndInitCall(fn);
}

/**
 * Run a function on the GLib event loop, yielding until the function
 * has finished.
 */
void GLibSignalHandler::gLibCall(std::function<void() > fn)
{
    //If already on the GLib thread the function can just run immediately.
    if (runningOnGLibThread())
    {
        fn();
    }
    else
    {
        const ScopedLock accessLock(threadLock);
        GLibThread* thread = static_cast<GLibThread*> (globalThread);
        GSource* trackedSource = addAndInitCall(fn);
        while (thread.callPending(trackedSource))
        {
            const ScopedUnlock threadUnlock(threadLock);
            Thread::yield();
        }
    }
}

/**
 * Initializes and starts the main GLib event loop on its own thread.
 */
GLibSignalHandler::GLibThread::GLibThread()
{
    mainLoop = g_main_loop_new(g_main_context_default(), false);
    startThread();
}

/**
 * Stops the event loop thread and cleans up all GLib resources.
 */
GLibSignalHandler::GlibThread::~GLibThread()
{
    g_main_context_wakeup(g_main_context_default());
    g_main_loop_quit(mainLoop);
    signalThreadShouldExit();
    waitForThreadToExit(-1);
    g_main_loop_unref(mainLoop);
    mainLoop = nullptr;
}

/**
 * Adds a function to the GMainContext so it will execute on the event
 * thread.
 */
GSource* GLibSignalHandler::GLibThread::addAndInitCall
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
 * Checks if a pending function is still waiting to execute.
 */
bool GLibSignalHandler::GLibThread::callPending(GSource* callSource)
{
    return sourceTracker.contains(callSource);
}

/**
 * Runs the GLib main loop.
 */
void GLibSignalHandler::GLibThread::run()
{
    if (context != nullptr)
    {
        g_main_loop_run(mainLoop);
    }
}

/**
 * Callback function used to execute arbitrary functions on the 
 * GMainLoop.
 */
gboolean GLibSignalHandler::GLibThread::runAsync
(GLibEventHandler::CallData* runData)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    runData->call();
    runData->sourceTracker->removeAllInstancesOf(runData->callSource);
    g_source_destroy(runData->callSource);
    delete runData;
    return true;
}