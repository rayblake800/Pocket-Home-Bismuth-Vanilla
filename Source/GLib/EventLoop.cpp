#include "GLib/EventLoop.h"

/*
 * Creates the event loop with an initial GMainContext.
 */
GLib::EventLoop::EventLoop(GMainContext* context) : context(context)
{  
    jassert(context != nullptr);
    mainLoop = g_main_loop_new(context, false);
}

/*
 * Unreferences the loop's GMainLoop and GMainContext.
 */
GLib::EventLoop::~EventLoop()
{
    // Never delete the event loop while it's running!
    jassert(!runningOnLoop());

    pendingCalls.getLock().enter();
    for(CallData* pending : pendingCalls)
    {
        if(pending->callerMutex != nullptr)
        {
            std::unique_lock<std::mutex> lock(*pending->callerMutex);
            pending->callPending->notify_one();
        }
    }

    if(mainLoop != nullptr)
    {
        g_main_loop_unref(mainLoop);
        mainLoop = nullptr;
    }
    if(context != nullptr)
    {
        g_main_context_unref(context);
        context = nullptr;
    }
    pendingCalls.getLock().exit();
}

/*
 * Checks if this is being executed within the event loop.
 */
bool GLib::EventLoop::runningOnLoop()
{
    return context != nullptr && g_main_context_is_owner(context);
}

/*
 * Runs a function on this GLib event loop, waiting until the function has 
 * finished.
 */
void GLib::EventLoop::call(std::function<void()> fn)
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
 * Gets the event loop's context.
 */
GMainContext* GLib::EventLoop::getContext()
{
    return context;
}

/*
 * Runs the GLib main loop.
 */
void GLib::EventLoop::runLoop()
{
    g_main_context_push_thread_default(context);
    DBG("GLib::EventLoop: entering GLib main loop");
    g_main_loop_run(mainLoop);
    DBG("GLib::EventLoop: exiting GLib main loop");
    g_main_context_pop_thread_default(context);
}

/*
 * Terminates the GLib main loop.
 */
void GLib::EventLoop::stopLoop()
{
    call([this]() 
        { 
            g_main_loop_quit(mainLoop); 
        });
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
