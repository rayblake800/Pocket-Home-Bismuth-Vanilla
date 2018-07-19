#include "GLibThread.h"

/**
 * @param context  The event context that will be claimed by this thread.
 *                 This will be unreferenced when the thread is destroyed.
 */
GLibThread::GLibThread(GMainContext* context) : Thread("GLibThread"),
context(context)
{
    
    startThread(); 
}

GLibThread::~GLibThread()
{
    addAndInitCall([this]()
    {
        g_main_loop_quit(mainLoop);
    });
    signalThreadShouldExit();
    waitForThreadToExit(-1);
    g_main_context_unref(context);
    context = nullptr;
    g_main_loop_unref(mainLoop);
    mainLoop = nullptr;
}

/**
 * Returns true if it's being executed on this GLib event thread.
 */
bool GLibThread::runningOnThread()
{
    return Thread::getCurrentThreadId() == getThreadId();
}

/**
 * Run a function on this GLib event loop, waiting until the function
 * has finished.
 */
void GLibThread::call(std::function<void()> fn)
{
    //If already on the GLib thread the function can just run immediately.
    if (runningOnThread())
    {
        fn();
    }
    else
    {
        std::mutex callMutex;
        std::condition_variable callPending;
        std::unique_lock<std::mutex> callLock(callMutex);
        addAndInitCall(fn, &callMutex, &callPending);
        callPending.wait(callLock);
    }
}

/**
 * Asynchronously run a function once on this GLib event loop.
 */
void GLibThread::callAsync(std::function<void()> fn)
{
    addAndInitCall(fn);
}

/**
 * Returns the thread default context that was set when this thread was
 * created.
 */
GMainContext* GLibThread::getContext()
{
    return context;
}

/**
 * Adds a function to the GMainContext so it will execute on the event
 * thread.
 */
void GLibThread::addAndInitCall(std::function<void() > call,
        std::mutex* callerMutex, std::condition_variable* callPending)
{
    if(threadShouldExit())
    {
        DBG("GLibThread::" << __func__ 
                << ": Thread is exiting, running function call now");
        call();
        return;
    }
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
    g_source_attach(callSource, context);
}

/**
 * Runs the GLib main loop.
 */
void GLibThread::run()
{
    if (context != nullptr)
    {
        g_main_context_push_thread_default(context);
        if(mainLoop == nullptr)
        {
            mainLoop = g_main_loop_new(context, false);
        }      
	DBG("GLibSignalHandler: entering GLib main loop");
        g_main_loop_run(mainLoop);
	DBG("GLibSignalHandler: exiting GLib main loop");
        g_main_context_pop_thread_default(context);
    }
}

/**
 * Callback function used to execute arbitrary functions on the 
 * GMainLoop.
 */
gboolean GLibThread::runAsync(CallData* runData)
{
    g_assert(g_main_context_is_owner(g_main_context_get_thread_default()));
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

/**
 * Pause the event loop whenever window focus is lost.
 */
void GLibThread::windowFocusLost()
{
    if(isThreadRunning())
    {
        addAndInitCall([this]()
        {
            g_main_loop_quit(mainLoop);
        });
        signalThreadShouldExit();
        waitForThreadToExit(-1);
    }
}

/*
 * Resume the event loop whenever window focus is regained.
 */
void GLibThread::windowFocusGained()
{
    if(mainLoop != nullptr)
    {
        startThread();
    }
}