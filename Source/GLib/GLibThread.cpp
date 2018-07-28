#include "GLibThread.h"


GLibThread::GLibThread(GMainContext* context) : Thread("GLibThread"),
context(context)
{  
    jassert(context != nullptr);
    startGLibThread(); 
}

GLibThread::~GLibThread()
{
    stopGLibThread(true);
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
    threadStateLock.enterRead();
    //If already on the GLib thread the function can just run immediately.
    if (runningOnThread())
    {
        fn();
    }
    else
    {
        if(mainLoop == nullptr || context == nullptr)
        {
            DBG("GLibThread::" << __func__ << ": GLib variables are null!");
            return;
        }
        
        bool threadStopped = !isThreadRunning();
        if(threadStopped)
        {
            DBG("GLibThread::" << __func__ 
                    << ": Thread not running, restarting thread.");
            threadStateLock.exitRead();
            if(!startGLibThread())
            {
                
                DBG("GLibThread::" << __func__  << ": Restarting thread failed,"
                        << " this should not happen!");
                return;
            }
            threadStateLock.enterRead();
        }    
        std::mutex callMutex;
        std::condition_variable callPending;
        std::unique_lock<std::mutex> callLock(callMutex);
        addAndInitCall(fn, &callMutex, &callPending);
        callPending.wait(callLock);
        if(threadStopped && !WindowFocus::isFocused())
        {
            DBG("GLibThread::" << __func__ 
                    << ": Window not focused, shutting down thread again.");
            threadStateLock.exitRead();
            stopGLibThread();
            return;
        }
    }
    threadStateLock.exitRead();
}

/**
 * Asynchronously run a function once on this GLib event loop.
 */
void GLibThread::callAsync(std::function<void()> fn)
{
    ScopedReadLock threadLock(threadStateLock);
    addAndInitCall(fn);
}

/**
 * Returns the thread default context that was set when this thread was
 * created.
 */
GMainContext* GLibThread::getContext()
{
    ScopedReadLock threadLock(threadStateLock);
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
        std::unique_lock<std::mutex> lock(threadStartMutex);
        mainLoop = g_main_loop_new(context, false);
        g_main_context_push_thread_default(context);
        threadStarting.notify_all();
        lock.unlock();
        
        DBG("GLibSignalHandler: entering GLib main loop");
        g_main_loop_run(mainLoop);
        DBG("GLibSignalHandler: exiting GLib main loop");
        
        g_main_context_pop_thread_default(context);
    }
    else
    {
        DBG("GLibThread::" << __func__ 
                << ": Thread started after the context was destroyed!");
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
    stopGLibThread();
}

/*
 * Resume the event loop whenever window focus is regained.
 */
void GLibThread::windowFocusGained()
{
    startGLibThread();
}

/**
 * Starts the GLib thread, then waits until the thread is running and
 * the thread context and main loop are initialized.  This function locks the
 * threadStateLock for writing.
 */
bool GLibThread::startGLibThread()
{
    ScopedWriteLock changeLock(threadStateLock);
    std::unique_lock<std::mutex> startLock(threadStartMutex);
    DBG("GLibThread::" << __func__ << ": Thread is starting.");
    if(isThreadRunning())
    {
        DBG("GLibThread::" << __func__ << ": Thread is already running.");
    }
    else
    {
        if(context == nullptr)
        {
            DBG("GLibThread::" << __func__ << ": Thread is being destroyed!");
            return false;
        }
        startThread();
        threadStarting.wait(startLock);      
    }
    return true;
}

/*
 * Terminates the GLib main loop and stops the thread.  This function locks the
 * threadStateLock for writing.
 */
void GLibThread::stopGLibThread(bool unrefGLibVars)
{
    ScopedWriteLock changeLock(threadStateLock);
    if(isThreadRunning())
    {
        DBG("GLibThread::" << __func__ << ": Thread is stopping.");
        if(mainLoop != nullptr && context != nullptr)
        {
            addAndInitCall([this]()
            {
                g_main_loop_quit(mainLoop);
            });
        }
        signalThreadShouldExit();
        waitForThreadToExit(-1);
    }
    if(unrefGLibVars)
    {
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
    }
}

