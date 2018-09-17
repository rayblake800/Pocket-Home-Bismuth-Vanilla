#include "GLibSignalThread.h"

static const juce::Identifier threadResourceKey = "GLibSignalThread";

/* Private SharedResource class */
class ThreadResource : public SharedResource, public GLibThread
{
public:
    ThreadResource() : SharedResource(threadResourceKey),
    GLibThread(g_main_context_default())
    {
        // Adding a reference to the default context prevents it from being 
        // destroyed when the GLibThread removes a reference on destruction.
        g_main_context_ref(g_main_context_default());
    }

    virtual ~ThreadResource() { }
};


GLibSignalThread::GLibSignalThread() : 
    ResourceHandler<ThreadResource>(threadResourceKey,
        []()->SharedResource* { return new ThreadResource(); })  { }

/*
 * Returns true if it's being executed on the GLib event thread.
 */
bool GLibSignalThread::runningOnGLibThread()
{
    auto thread = getReadLockedResource();
    return thread->runningOnThread();
}
   
/**
 * Asynchronously run a function once on the GLib event loop.
 */
void GLibSignalThread::gLibCallAsync(std::function<void() > fn)
{
    auto thread = getWriteLockedResource();
    thread->callAsync(fn);
}

/**
 * Run a function on the GLib event loop, yielding until the function
 * has finished.
 */
void GLibSignalThread::gLibCall(std::function<void() > fn)
{
    auto thread = getWriteLockedResource();
    thread->call(fn);
}
