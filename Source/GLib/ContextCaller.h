/* Since the main UI/message thread isn't a Juce thread, standard C++ thread
   libraries need to be used to wait/notify. */
#include <mutex>
#include <condition_variable>
#include "JuceHeader.h"
#include "GLib/EventLoop.h"
#include "GLib/GLib.h"


/**
 * @file  GLib/ContextCaller.h
 *
 * @brief  Runs functions synchronously and asynchronously within a GLib event
 *         loop attached to a specific GLib context.
 *
 *  The ContextCaller will not run the event loop itself, or ensure that the 
 * loop is running. That should be handled elsewhere, probably by a
 * GLib::ThreadResource object.
 *
 */
class GLib::ContextCaller
{
public:
    /**
     * @brief  Initializes the ContextCaller, setting its GLib event loop.
     *
     * @param eventLoop  A reference to the eventLoop where the ContextCaller
     *                   will schedule function calls.
     */
    ContextCaller(const EventLoop& eventLoop);

    /**
     * @brief  Cancels all pending calls when the ContextCaller is destroyed.
     */
    virtual ~ContextCaller();
    
    void callAsync(std::function<void()> toCall,
            std::function<void()> onFailure = std::function<void()>());

    void call(std::function<void()> toCall,
            std::function<void()> onFailure = std::function<void()>(),
            std::function<void()> afterAdding = std::function<void()>());

private:

    struct CallData
    {
        ContextCaller* caller;
        std::function<void()> toCall;
        std::function<void()> onFailure;
        GSource* callSource;
        std::mutex* callerMutex = nullptr;
        std::condition_variable* callPending = nullptr;
    };
    
    /**
     * @brief  Adds a function to the GMainContext so it will execute on the 
     *         event loop.
     * 
     * @param call         The function to run.
     * 
     * @param callerMutex  If this value is non-null, the EventLoop will
     *                     lock it while running this call.
     * 
     * @param callPending  If this value is non-null, the EventLoop will
     *                     use it and callerMutex to wake up the calling
     *                     thread after running the call.
     */
    void addAndInitCall(std::function<void() > call,
            std::mutex* callerMutex = nullptr,
            std::condition_variable* callPending = nullptr);
    /**
     * @brief  A callback function used to execute arbitrary functions within
     *         the context's GMainLoop.
     *
     * @param runData  Holds the data needed to call the function and possibly
     *                 notify the function's source.
     *
     * @return         False.
     *                 TODO: Find and document why this always should return
     *                       false. I know I had a good reason for this, and it
     *                       works correctly, but I forgot to document why, and
     *                       GLib callback function documentation is poorly
     *                       documented and hard to find.
     */
    static gboolean runAsync(CallData* runData);

    /* Holds pending calls, ensuring CallData is not leaked if the EventLoop
       is destroyed before a call can run. */
    juce::OwnedArray<CallData, juce::CriticalSection> pendingCalls;

    /* The GLib event loop where the ContextCaller schedules calls. */
    const EventLoop& eventLoop;
};
