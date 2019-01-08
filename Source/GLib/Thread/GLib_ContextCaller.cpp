#include "GLib_ContextCaller.h"

/*
 * Initializes the ContextCaller, setting its GLib main context.
 */
GLib::ContextCaller::ContextCaller(const SharedContextPtr& contextPtr) : 
    contextPtr(contextPtr) { }

/*
 * Cancels all pending calls when the ContextCaller is destroyed.
 */
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
        else if(pending->onFailure)
        {
            pending->onFailure();
        }
    }
}

/*
 * Schedules a function call to run asynchronously within the context's event 
 * loop. 
 */
void GLib::ContextCaller::callAsync(std::function<void()> toCall,
        std::function<void()> onFailure)
{
    addAndInitCall(toCall, onFailure);
}

/*
 * Schedules a function call to run within the context's event loop, then waits 
 * for the function to execute.
 */
void GLib::ContextCaller::call(std::function<void()> toCall, 
        std::function<void()> onFailure, std::function<void()> afterAdding)
{
    // If already in the GLib EventLoop the function can just run immediately.
    if(g_main_context_is_owner(*contextPtr))
    {
        toCall();
    }
    else
    {
        if(contextPtr == nullptr)
        {
            DBG("GLib::EventLoop::" << __func__ << ": GLib context is null!");
            jassertfalse;
            return;
        }
        std::mutex callMutex;
        std::condition_variable callPending;
        std::unique_lock<std::mutex> callLock(callMutex);
        addAndInitCall(toCall, onFailure, &callMutex, &callPending);
        if(afterAdding)
        {
            afterAdding();
        }
        callPending.wait(callLock);
    }
}


/*
 * Adds a function to the GMainContext so it will execute on the event loop.
 */
void GLib::ContextCaller::addAndInitCall(std::function<void()> toCall,
        std::function<void()> onFailure,
        std::mutex* callerMutex, std::condition_variable* callPending)
{
    jassert((callerMutex == nullptr && callPending == nullptr)
            || (callerMutex != nullptr && callPending != nullptr));
    CallData* callData = new CallData;
    callData->caller = this;
    callData->toCall = toCall;
    callData->onFailure = onFailure;
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
    g_source_attach(callSource, *contextPtr);
}

/*
 * A callback function used to execute arbitrary functions on the GMainLoop.
 */
gboolean GLib::ContextCaller::runAsync(CallData* runData)
{
    jassert(g_main_context_is_owner(*runData->caller->contextPtr));
    if(runData->callerMutex != nullptr)
    {
        std::unique_lock<std::mutex> lock(*runData->callerMutex);
        runData->toCall();
        runData->callPending->notify_one();
    }
    else
    {
        runData->toCall();
    }
    g_source_destroy(runData->callSource);
    ContextCaller* caller = runData->caller;

    caller->pendingCalls.getLock().enter();
    caller->pendingCalls.removeObject(runData);
    runData = nullptr;
    caller->pendingCalls.getLock().exit();

    return false;
}
