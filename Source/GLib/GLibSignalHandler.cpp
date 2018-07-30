#include "GLibSignalHandler.h"


juce::ScopedPointer<ResourceManager::SharedResource> 
GLibSignalHandler::globalThread;

juce::ReadWriteLock GLibSignalHandler::threadLock;

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
    using namespace juce;
    const ScopedReadLock accessLock(threadLock);
    GLibDefaultThread* thread
            = static_cast<GLibDefaultThread*> (globalThread.get());
    return thread->runningOnThread();
}
   
/**
 * Asynchronously run a function once on the GLib event loop.
 */
void GLibSignalHandler::gLibCallAsync(std::function<void() > fn)
{
    using namespace juce;
    const ScopedWriteLock accessLock(threadLock);
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
    threadLock.enterWrite();
    GLibDefaultThread* thread 
            = static_cast<GLibDefaultThread*> (globalThread.get());
    //synchronous calls will lock the thread themselves
    threadLock.exitWrite();
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
