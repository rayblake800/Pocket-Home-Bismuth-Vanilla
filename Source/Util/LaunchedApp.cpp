#include "LaunchedApp.h"
#include "JuceHeader.h"
#include "ProcessUtils.h"
#include "XWindowInterface.h"
#include <cstdint>

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
        outputValid = false;
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
    bool processRunning = childProcess.isRunning();
    if(processRunning)
    {
        ProcessUtils::ProcessData process 
                = ProcessUtils::getProcessData(processId);
        DBG("LaunchedApp::" << __func__ << ": process " << launchCommand
                << " is in state " 
                << ProcessUtils::processStateString(process.lastState));
        if(process.lastState == ProcessUtils::ProcessState::dead
           || process.lastState == ProcessUtils::ProcessState::invalid )
        {
            bool killed = kill();
            jassert(killed);
            processRunning = false;
        }
    }  
    return processRunning;
}

/*
 * Attempts to terminate the application process.
 */
bool LaunchedApp::kill()
{
    outputValid = false;
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
    if(!outputValid)
    {
        return String();
    }
    if(!isRunning())
    {
        return childProcess.readAllProcessOutput();
    }
    else
    {    
        ProcessUtils::ProcessData process 
                = ProcessUtils::getProcessData(processId);
        DBG("LaunchedApp::" << __func__ << ": process " << launchCommand
                << " is still active in state " 
                << ProcessUtils::processStateString(process.lastState)
                << ", can't get output");
        return String();
    }
}


    
/*
 * If the process is finished, return its exit code.
 */
juce::uint32 LaunchedApp::getExitCode()
{
    if(isRunning())
    {
        return UINT32_MAX;
    }
    return childProcess.getExitCode();
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
        
    XWindowInterface xWindows;
    Array<Window> appWindows = xWindows.getMatchingWindows([this, &xWindows]
        (Window window)
        {
            return xWindows.getWindowPID(window) == processId
                    && xWindows.getWindowName(window).isNotEmpty()
                    && xWindows.getWindowDesktop(window) != -1;
        }, false);
    if(appWindows.isEmpty())
    {
        DBG("LaunchedApp::" << __func__ << ": no windows found!");
        return;
    }
    for(const Window& window : appWindows)
    {
        //raise and activate entire window line, ensuring each of the window's
        //parents is the topmost window
        Array<Window> ancestry = xWindows.getWindowAncestry(window);
        jassert(!ancestry.isEmpty());
        String name = xWindows.getWindowName(window);
        String winClass = xWindows.getWindowClass(window);
        String className = xWindows.getWindowClassName(window);
        int desktop = xWindows.getWindowDesktop(window);
        int pid = xWindows.getWindowPID(window);
        DBG("LaunchedApp::" << __func__ << "Activating window, name=" 
                << name << " class=" << winClass
                << " className=" << className << " desktop=" << desktop
                << " pid=" << pid << " depth=" << ancestry.size());
        for(const Window& window : ancestry)
        {
            xWindows.activateWindow(window);
        }
    }
    
}
