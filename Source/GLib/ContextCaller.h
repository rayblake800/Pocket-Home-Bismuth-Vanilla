/* Since the main UI/message thread isn't a Juce thread, standard C++ thread
   libraries need to be used to wait/notify. */
#include <mutex>
#include <condition_variable>
#include "JuceHeader.h"
#include "GLib/SmartPointers/MainContextPtr.h"
#include "GLib/GLib.h"


/**
 * @file  GLib/ContextCaller.h
 *
 * @brief  Runs functions synchronously and asynchronously within a GLib event
 *         loop attached to a specific GLib context.
 *
 * ContextCaller does nothing to ensure that its GMainContext is connected to a
 * valid GLib event loop. 
 */
class GLib::ContextCaller
{
public:
    /**
     * @brief  Initializes the ContextCaller, setting its GLib context.
     *
     * @param context  A non-null GLib context pointer, with an extra reference
     *                 the ContextCaller can unreference when it is destroyed.
     */
    ContextCaller(GMainContext* context);

    virtual ~ContextCaller();
    
    void callAsync(std::function<void()> toCall,
            std::function<void()> onFailure = std::function<void()>(),
            std::function<void()> afterAdding = std::function<void()>());

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

    /* The GLib context object used to schedule functions on the event loop. */ 
    const MainContextPtr context;
};
