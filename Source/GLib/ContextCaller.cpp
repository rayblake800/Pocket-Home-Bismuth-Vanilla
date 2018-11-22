#include "GLib/ContextCaller.h"

GLib::ContextCaller::ContextCaller(GMainContext* context) : context(context) { }

GLib::ContextCaller::~ContextCaller()
{
    const juce::ScopedLock clearLock(pendingCalls.getLock());
    for(CallData* pending : pendingCalls)
    {
        if(pending->callerMutex != nullptr)
        {
            std::unique_lock<std::mutex> lock(*pending->callerMutex);
            if(pending->onFailure)
            {
                pending->onFailure();
            }
            pending->callPending->notify_one();
        }
    }
}

/*
 * Runs a function on this GLib event loop, waiting until the function has 
 * finished.
 */
void GLib::ContextCaller::call(std::function<void()> fn)
{
    // If already in the GLib EventLoop the function can just run immediately.
    if (runningOnLoop())
    {
        fn();
    }
    else
    {
        if(mainLoop == nullptr || context == nullptr)
        {
            DBG("GLib::EventLoop::" << __func__ 
                    << ": GLib variables are null!");
            jassertfalse;
            return;
        }
        std::mutex callMutex;
        std::condition_variable callPending;
        std::unique_lock<std::mutex> callLock(callMutex);
        addAndInitCall(fn, &callMutex, &callPending);
        callPending.wait(callLock);
    }
}

/*
 * Asynchronously run a function once on this GLib event loop.
 */
void GLib::EventLoop::callAsync(std::function<void()> fn)
{
    addAndInitCall(fn);
}

/*
 * Adds a function to the GMainContext so it will execute on the event loop.
 */
void GLib::EventLoop::addAndInitCall(std::function<void() > call,
        std::mutex* callerMutex, std::condition_variable* callPending)
{
    jassert((callerMutex == nullptr && callPending == nullptr)
            || (callerMutex != nullptr && callPending != nullptr));
    CallData* callData = new CallData;
    callData->eventLoop = this;
    callData->call = call;
    GSource* callSource = g_idle_source_new();
    callData->callSource = callSource;
    callData->callerMutex = callerMutex;
    callData->callPending = callPending;

    pendingCalls.getLock().enter();
    pendingCalls.add(callData);
    pendingCalls.getLock().exit();

    g_source_set_callback(
            callSource,
            (GSourceFunc) runAsync,
            callData,
            nullptr);
    g_source_attach(callSource, context);
}

/*
 * A callback function used to execute arbitrary functions on the GMainLoop.
 */
gboolean GLib::EventLoop::runAsync(CallData* runData)
{
    jassert(g_main_context_is_owner(g_main_context_get_thread_default()));
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
    EventLoop* eventLoop = runData->eventLoop;

    eventLoop->pendingCalls.getLock().enter();
    eventLoop->pendingCalls.removeObject(runData);
    runData = nullptr;
    eventLoop->pendingCalls.getLock().exit();

    return false;
}
