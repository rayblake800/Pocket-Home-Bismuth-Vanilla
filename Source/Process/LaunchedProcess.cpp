#include "LaunchedProcess.h"
#include "JuceHeader.h"
#include "ProcessUtils.h"
#include "XWindowInterface.h"
#include <cstdint>

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "LaunchedProcess::";
#endif

/* If a juce::ChildProcess crashes, attempts to wait for it to terminate will
 * wait forever if not given a timeout period. Instead of waiting indefinitely,
 * LaunchedProcess will repeatedly wait for processCheckInterval milliseconds,
 * checking the process state between waiting periods.
 */
static const constexpr int processCheckInterval = 500;

/*
 * Creates a LaunchedProcess object, running an application launch command in a
 * new child process.
 */
LaunchedProcess::LaunchedProcess(juce::String launchCommand) :
launchCommand(launchCommand) 
{
    if(childProcess.start(launchCommand))
    {
        juce::Array<ProcessUtils::ProcessData> childProcs
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
        DBG(dbgPrefix << __func__ << ": Failed to launch " << launchCommand);
        outputValid = false;
    }
}

/*
 * Gets the launch command used to start the process.
 */
juce::String LaunchedProcess::getLaunchCommand()
{
    return launchCommand;
}

/*
 * Checks if the launched application is still running.
 */
bool LaunchedProcess::isRunning()
{
    bool processRunning = childProcess.isRunning();
    if(processRunning)
    {
        ProcessUtils::ProcessData process 
                = ProcessUtils::getProcessData(processId);
        DBG(dbgPrefix << __func__ << ": process " << launchCommand
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
bool LaunchedProcess::kill()
{
    outputValid = false;
    return childProcess.kill();
}

/*
 * Waits for the application process to end.
 */
void LaunchedProcess::waitForProcessToFinish(int timeoutMs)
{
    bool indefiniteWait = (timeoutMs == -1);
    juce::uint32 endTime = juce::Time::getMillisecondCounter() + timeoutMs;
    ProcessUtils::ProcessData process = ProcessUtils::getProcessData(processId);
    while(childProcess.isRunning()
            && process.lastState != ProcessUtils::ProcessState::zombie
            && process.lastState != ProcessUtils::ProcessState::stopped
            && process.lastState != ProcessUtils::ProcessState::dead
            && process.lastState != ProcessUtils::ProcessState::unknown
            && process.lastState != ProcessUtils::ProcessState::invalid
            && (indefiniteWait
                || juce::Time::getMillisecondCounter() < endTime))

    {
        int waitTime = processCheckInterval;
        if(!indefiniteWait)
        {
            juce::uint32 timeLeft = endTime 
                    - juce::Time::getMillisecondCounter();
            if(timeLeft < waitTime)
            {
                waitTime = timeLeft;
            }
        }
        childProcess.waitForProcessToFinish(waitTime);
        process = ProcessUtils::getProcessData(processId);
    }
#ifdef JUCE_DEBUG
    if(juce::Time::getMillisecondCounter() >= endTime
            && childProcess.isRunning())
    {
        DBG(dbgPrefix << __func__ << ": process " << launchCommand
                << " did not finish, process state="
                << ProcessUtils::processStateString(process.lastState));
    }
#endif
}

/*
 * Gets all text output by the process to stdout and stderr.
 */
juce::String LaunchedProcess::getProcessOutput()
{
    if(!outputValid)
    {
        return juce::String();
    }
    if(!isRunning())
    {
        return childProcess.readAllProcessOutput();
    }
    else
    {    
        ProcessUtils::ProcessData process 
                = ProcessUtils::getProcessData(processId);
        DBG(dbgPrefix << __func__ << ": process " << launchCommand
                << " is still active in state " 
                << ProcessUtils::processStateString(process.lastState)
                << ", can't get output");
        return juce::String();
    }
}
    
/*
 * If the process is finished, return its exit code.
 */
juce::uint32 LaunchedProcess::getExitCode()
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
void LaunchedProcess::activateWindow()
{
    if(!isRunning())
    {
        DBG(dbgPrefix << __func__ << ": application is not running!");
        return;
    }
    if(processId == -1)
    {
        DBG(dbgPrefix << __func__ << ": process is not found!");
        return;
    }
        
    XWindowInterface xWindows;
    juce::Array<Window> appWindows = xWindows.getMatchingWindows(
            [this, &xWindows] (Window window)
    {
        return xWindows.getWindowPID(window) == processId
                && xWindows.getWindowName(window).isNotEmpty()
                && xWindows.getWindowDesktop(window) != -1;
    }, false);
    if(appWindows.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": no windows found!");
        return;
    }
    for(const Window& window : appWindows)
    {
        DBG(dbgPrefix << __func__ << ": Activating window for " 
                << launchCommand);
        xWindows.activateWindow(window);
    }
}
