#include "Utils.h"

#if JUCE_DEBUG

static std::map<const void*,int>& getAddressMap()
{
    static std::map<const void*,int> ids;
    return ids;
}

/*
 * Convert a pointer to a unique, fixed ID for debug output.
 */
int addressID(const void* ptr)
{
    static int nextID = 0;
    std::map<const void*,int>& ids = getAddressMap();
    if(nextID == 0)
    {
        ids[0] = 0;
        nextID++;
    }
    if(ids.count(ptr) == 0)
    {
        ids[ptr] = nextID;
        nextID++;
    }
    return ids[ptr];
}

/*
 * Appends a line of text to the log of events occurring to a specific address.
 */
const juce::String& addressLog
(const void* ptr, juce::String event, const void* ptr2)
{
    using namespace juce;
    static CriticalSection logSection;
    static std::map<int,String> eventLog;
    const ScopedLock eventLock(logSection);
    if(ptr == nullptr)
    {
        return eventLog[0];
    }
    int id = addressID(ptr);
    String& log = eventLog[id];
    log += "\n";
    log += event;
    if(ptr2 != nullptr)
    {
        log+=addressID(ptr2);
    }
    return log;
}

/*
 * Prints all logged events for a specific memory address
 */
void printLog(int addressID)
{
    using namespace juce;
    const void* address = nullptr;
    std::map<const void*,int>& ids = getAddressMap();
    for(auto iter = ids.begin(); iter != ids.end(); iter++)
    {
        if(iter->second == addressID)
        {
            address = iter->first;
            break;
        }
    }
    const String& log = ADDR_LOG(address, "printed log");
    std::cout << "\n\n\tPrinting log for " << addressID << ":\n"
            << log << "\n\n";
}

#endif

/*
 * Waits for a process to end, forcibly terminating it if necessary, then
 * returns all process output as long as the process exited normally.
 */
juce::String getProcessOutput
(juce::ChildProcess& process, unsigned int timeoutMs)
{
    using namespace juce;
    process.waitForProcessToFinish(timeoutMs);
    if(!process.isRunning())
    {
        return process.readAllProcessOutput();
    }
    if(process.isRunning() && !process.kill())
    {
        DBG(__func__ << ": Child process refused to stop!");
    }
    return String();
}

/*
 * Requests user confirmation before performing some action
 */
void confirmAction
(juce::String title, juce::String message, std::function<void() > onConfirm)
{
    using namespace juce;
    NativeMessageBox::showOkCancelBox(AlertWindow::QuestionIcon,
            title, message, nullptr,
            ModalCallbackFunction::create([onConfirm](int response)
            {
                if (response == 1)
                {
                    onConfirm();
                }
            }));
}


/**
 * Gets the size of the active display.
 * 
 * @return the size of the display, measured in pixels.
 */
juce::Rectangle<int> getDisplaySize(){
    using namespace juce;
    return Desktop::getInstance().getDisplays().getMainDisplay().userArea;
}

/**
 * Gets the bounds of the single application window.
 */
juce::Rectangle<int> getWindowBounds()
{
    using namespace juce;
    Component * windowComp = Desktop::getInstance().getComponent(0);
    if (windowComp == nullptr)
    {
        return Rectangle<int>();
    }
    return windowComp->getBounds();
}
