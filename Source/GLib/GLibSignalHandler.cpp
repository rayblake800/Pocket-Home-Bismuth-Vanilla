#include "GLibSignalHandler.h"


ScopedPointer<ResourceManager::SharedResource> GLibSignalHandler::globalThread;
CriticalSection GLibSignalHandler::threadLock;

GLibSignalHandler::GLibSignalHandler()
: ResourceManager(globalThread, threadLock, []()
{

    return new GLibDefaultThread();
}) { }

/**
 * Returns true if it's being executed on the GLib event thread.
 */
bool GLibSignalHandler::runningOnGLibThread()
{
    const ScopedLock accessLock(threadLock);
    GLibDefaultThread* thread
            = static_cast<GLibDefaultThread*> (globalThread.get());
    return thread->runningOnThread();
}
   
/**
 * Asynchronously run a function once on the GLib event loop.
 */
void GLibSignalHandler::gLibCallAsync(std::function<void() > fn)
{
    const ScopedLock accessLock(threadLock);
    GLibDefaultThread* thread
            = static_cast<GLibDefaultThread*> (globalThread.get());
    thread->callAsync(fn);
}

/**
 * Run a function on the GLib event loop, yielding until the function
 * has finished.
 */
void GLibSignalHandler::gLibCall(std::function<void() > fn)
{
    const ScopedLock accessLock(threadLock);
    GLibDefaultThread* thread 
            = static_cast<GLibDefaultThread*> (globalThread.get());
    //synchronous calls will lock the thread themselves
    const ScopedUnlock callUnlock(threadLock);
    thread->call(fn);
}

/**
 * Initializes and starts the main GLib event loop on its own thread.
 */
GLibSignalHandler::GLibDefaultThread::GLibDefaultThread() : 
GLibThread(g_main_context_default())
{
    //Adding a reference to the default context prevents it from being 
    //destroyed when the GLibThread removes a reference on destruction.
    g_main_context_ref(g_main_context_default());
}
