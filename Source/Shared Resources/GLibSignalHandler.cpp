#include "GLibSignalHandler.h"


ScopedPointer<ResourceManager::SharedResource> GLibSignalHandler::globalThread;
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
    GLibThread* thread = static_cast<GLibThread*> (globalThread.get());
    return Thread::getCurrentThreadId() == thread->getThreadId();
}
   
/**
 * Asynchronously run a function once on the GLib event loop.
 */
void GLibSignalHandler::gLibCallAsync(std::function<void() > fn)
{
    const ScopedLock accessLock(threadLock);
    GLibThread* thread = static_cast<GLibThread*> (globalThread.get());
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
        GLibThread* thread = static_cast<GLibThread*> (globalThread.get());
        std::mutex callMutex;
        std::condition_variable callPending;
        std::unique_lock<std::mutex> callLock(callMutex);
        thread->addAndInitCall(fn, &callMutex, &callPending);
        const ScopedUnlock threadUnlock(threadLock);
        callPending.wait(callLock);
    }
}

/**
 * Initializes and starts the main GLib event loop on its own thread.
 */
GLibSignalHandler::GLibThread::GLibThread() : Thread("GLibThread")
{
    mainLoop = g_main_loop_new(g_main_context_default(), false);
    startThread();
}

/**
 * Stops the event loop thread and cleans up all GLib resources.
 */
GLibSignalHandler::GLibThread::~GLibThread()
{
    addAndInitCall([this]()
    {
        g_main_loop_quit(mainLoop);
    });
    signalThreadShouldExit();
    waitForThreadToExit(-1);
    g_main_loop_unref(mainLoop);
    mainLoop = nullptr;
}

/**
 * Adds a function to the GMainContext so it will execute on the event
 * thread.
 */
void GLibSignalHandler::GLibThread::addAndInitCall
(std::function<void() > call, std::mutex* callerMutex,
        std::condition_variable* callPending)
{
    jassert((callerMutex == nullptr && callPending == nullptr)
            || (callerMutex != nullptr && callPending != nullptr));
    CallData* callData = new CallData;
    callData->call = call;
    GSource* callSource = g_idle_source_new();
    callData->callSource = callSource;
    callData->callerMutex = callerMutex;
    callData->callPending = callPending;
    g_source_set_callback(
            callSource,
            (GSourceFunc) runAsync,
            callData,
            nullptr);
    g_source_attach(callSource, g_main_context_default());
}
        
/**
 * Runs the GLib main loop.
 */
void GLibSignalHandler::GLibThread::run()
{
    g_main_context_acquire(g_main_context_default());
    if (g_main_context_default() != nullptr)
    {
        g_main_loop_run(mainLoop);
	DBG("GLibSignalHandler: exiting GLib main loop");
    }
}

/**
 * Callback function used to execute arbitrary functions on the 
 * GMainLoop.
 */
gboolean GLibSignalHandler::GLibThread::runAsync
(GLibSignalHandler::GLibThread::CallData* runData)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    if(runData->callerMutex != nullptr)
    {
        std::unique_lock<std::mutex> lock(*runData->callerMutex);
        runData->call();
        runData->callPending->notify_one();
    }
    else
    {
        runData->call();
    }
    g_source_destroy(runData->callSource);
    delete runData;
    return false;
}
