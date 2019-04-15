#include "GLib_EventLoop.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "GLib::EventLoop::";
#endif

// Creates the event loop with an initial GMainContext.
GLib::EventLoop::EventLoop(const SharedContextPtr& context) : context(context),
    eventLoop(g_main_loop_new(*context, false)) { }


// Ensures the loop has stopped before it is destroyed.
GLib::EventLoop::~EventLoop()
{
    // Never delete the loop from within the loop!
    jassert(!runningOnLoop());
    while (eventLoop != nullptr && g_main_loop_is_running(eventLoop))
    {
        stopLoop();
        juce::Thread::yield();
    }
}


// Checks if this is being executed within the event loop.
bool GLib::EventLoop::runningOnLoop()
{
    return context != nullptr && g_main_context_is_owner(*context);
}


// Gets the event loop's context.
GLib::SharedContextPtr GLib::EventLoop::getContext() const
{
    return context;
}


// Runs the GLib main loop.
void GLib::EventLoop::runLoop()
{
    g_main_context_push_thread_default(*context);
    DBG(dbgPrefix << __func__ << ": entering GLib main loop");
    g_main_loop_run(eventLoop);
    DBG(dbgPrefix << __func__ << ": exiting GLib main loop");
    g_main_context_pop_thread_default(*context);
}


// Terminates the GLib main loop.
void GLib::EventLoop::stopLoop()
{
    g_main_loop_quit(eventLoop);
}
