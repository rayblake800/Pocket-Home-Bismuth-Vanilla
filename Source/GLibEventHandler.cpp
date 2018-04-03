#include "GLibEventHandler.h"


ScopedPointer<ResourceManager::SharedResource> GLibEventHandler::threadResource;
CriticalSection GLibEventHandler::threadLock;

GLibEventHandler::GLibEventHandler()
: ResourceManager(threadResource, threadLock, [this]()
{

    return new GLibThread();
}) { }

/**
 * Asynchronously run a function once on the GLib event loop.
 */
void GLibEventHandler::callAsyncGLib(std::function<void() > fn)
{
    ScopedLock accessLock(threadLock);
    GLibThread* thread = static_cast<GLibThread*> (threadResource.get());
    thread->addAndInitCall(fn);
}

/**
 * Run a function on the GLib event loop, blocking until the function
 * finishes.
 */
void GLibEventHandler::callGLib(std::function<void() > fn)
{
    ScopedLock accessLock(threadLock);
    GLibThread* thread = static_cast<GLibThread*> (threadResource.get());
    GSource* trackedSource = thread->addAndInitCall(fn);
    while (thread->callIsPending(trackedSource))
    {
        ScopedUnlock yieldUnlock(threadLock);
        Thread::yield();
    }
}

GLibEventHandler::GLibThread::GLibThread() : Thread("GLibThread")
{
    context = g_main_context_new();
    mainLoop = g_main_loop_new(context,false);
    startThread();
}

GLibEventHandler::GLibThread::~GLibThread()
{
    g_main_loop_quit(mainLoop);
    signalThreadShouldExit();
    waitForThreadToExit(-1);
    g_main_loop_unref(mainLoop);
    g_main_context_unref(context);
    mainLoop = nullptr;
    context = nullptr;
}

GSource* GLibEventHandler::GLibThread::addAndInitCall
(std::function<void() > call)
{
    CallbackData* callData = new CallbackData;
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

gboolean GLibEventHandler::GLibThread::runAsync
(GLibEventHandler::GLibThread::CallbackData* runData)
{
    runData->call();
    runData->sourceTracker->removeAllInstancesOf(runData->callSource);
    g_source_destroy(runData->callSource);
    delete runData;
    return true;
}

void GLibEventHandler::GLibThread::run()
{
    if (context != nullptr)
    {
        g_main_context_push_thread_default(context);
        g_main_loop_run(mainLoop);
    }
}