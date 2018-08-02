#include "LaunchedApp.h"
#include "JuceHeader.h"
#include "ProcessUtils.h"
#include "XWindowInterface.h"

/*
 * Creates a LaunchedApp object, running an application launch command in a
 * new child process.
 */
LaunchedApp::LaunchedApp(juce::String launchCommand) :
launchCommand(launchCommand) 
{
    using namespace juce;
    if(childProcess.start(launchCommand))
    {
        Array<ProcessUtils::ProcessData> childProcs
                = ProcessUtils::getChildProcesses(ProcessUtils::getProcessId());
        jassert(!childProcs.isEmpty());
        for(const ProcessUtils::ProcessData process : childProcs)
        {
            if(launchCommand.contains(process.executableName))
            {
                processId = process.processId;    
                break;
            }
        }
    }
    else
    {
        DBG("LaunchedApp::" << __func__ << ": Failed to launch "
                            << launchCommand);
    }
}

/*
 * Gets the launch command used to start the process.
 */
juce::String LaunchedApp::getLaunchCommand()
{
    return launchCommand;
}

/*
 * Checks if the launched application is still running.
 */
bool LaunchedApp::isRunning()
{
    return childProcess.isRunning();
}

/*
 * Attempts to terminate the application process.
 */
bool LaunchedApp::kill()
{
    return childProcess.kill();
}

/*
 * Waits for the application process to end.
 */
void LaunchedApp::waitForProcessToFinish(int timeoutMs)
{
    childProcess.waitForProcessToFinish(timeoutMs);
}

/*
 * Gets all text output by the process to stdout and stderr.
 */
juce::String LaunchedApp::getProcessOutput()
{
    using namespace juce;
    ProcessUtils::ProcessData process = ProcessUtils::getProcessData(processId);
    switch(process.lastState)
    {
        case ProcessUtils::running:
            DBG("LaunchedApp::" << __func__ << ": Process is running.");
            break;
        case ProcessUtils::sleep:
            DBG("LaunchedApp::" << __func__ << ": Process is sleeping.");
            break;
        case ProcessUtils::uninterruptableSleep:
            DBG("LaunchedApp::" << __func__ 
                    << ": Process is in uninterruptable sleep.");
            break;
        case ProcessUtils::stopped:
            DBG("LaunchedApp::" << __func__ << ": Process is stopped.");
            break;
        case ProcessUtils::dead:
            DBG("LaunchedApp::" << __func__ << ": Process is dead.");
            break;
        case ProcessUtils::nonexistent:
            DBG("LaunchedApp::" << __func__ << ": Process does not exist!");
            return String();
    }
    return childProcess.readAllProcessOutput();
}

/*
 * Moves the application's windows in front of all other windows and focuses
 * them.
 */
void LaunchedApp::activateWindow()
{
    using namespace juce;
    if(!isRunning())
    {
        DBG("LaunchedApp::" << __func__ << ": application is not running!");
        return;
    }
    if(processId == -1)
    {
        DBG("LaunchedApp::" << __func__ << ": process is not found!");
        return;
    }
    ProcessUtils::ProcessData process = ProcessUtils::getProcessData(processId);
    if(process.lastState == ProcessUtils::nonexistent
       || process.lastState == ProcessUtils::dead)
    {
        DBG("LaunchedApp::" << __func__ << ": process is in an invalid state!");
        return;
    }
        
    XWindowInterface xWindows;
    Array<Window> appWindows = xWindows.getMatchingWindows([this, &xWindows]
        (Window window)
        {
            return xWindows.getWindowPID(window) == processId;
        });
    if(appWindows.isEmpty())
    {
        DBG("LaunchedApp::" << __func__ << ": no windows found!");
        return;
    }
    DBG("LaunchedApp::" << __func__ << ": activating " << appWindows.size()
            << " windows for application " << process.executableName);
    for(const Window& window : appWindows)
    {
        xWindows.activateWindow(window);
    }
    
}
