#include "LaunchedProcess.h"
#include "JuceHeader.h"
#include "ProcessUtils.h"
#include "XWindowInterface.h"
#include <cstdint>

/*
 * If a juce::ChildProcess crashes, attempts to wait for it to terminate will
 * wait forever if not given a timeout period.  Instead of waiting indefinitely,
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
        DBG("LaunchedProcess::" << __func__ << ": Failed to launch "
                            << launchCommand);
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
        DBG("LaunchedProcess::" << __func__ << ": process " << launchCommand
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
    using namespace juce;
    bool indefiniteWait = (timeoutMs == -1);
    uint32 endTime = juce::Time::getMillisecondCounter() + timeoutMs;
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
            uint32 timeLeft = endTime - juce::Time::getMillisecondCounter();
            if(timeLeft < waitTime)
            {
                waitTime = timeLeft;
            }
        }
        childProcess.waitForProcessToFinish(waitTime);
        process = ProcessUtils::getProcessData(processId);
    }
    if(childProcess.isRunning())
    {
        DBG("LaunchedProcess::" << __func__ << ": process " << launchCommand
                << " did not finish, process state="
                << ProcessUtils::processStateString(process.lastState));
    }
}

/*
 * Gets all text output by the process to stdout and stderr.
 */
juce::String LaunchedProcess::getProcessOutput()
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
        DBG("LaunchedProcess::" << __func__ << ": process " << launchCommand
                << " is still active in state " 
                << ProcessUtils::processStateString(process.lastState)
                << ", can't get output");
        return String();
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
    using namespace juce;
    if(!isRunning())
    {
        DBG("LaunchedProcess::" << __func__ << ": application is not running!");
        return;
    }
    if(processId == -1)
    {
        DBG("LaunchedProcess::" << __func__ << ": process is not found!");
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
        DBG("LaunchedProcess::" << __func__ << ": no windows found!");
        return;
    }
    for(const Window& window : appWindows)
    {
        DBG("LaunchedProcess::" << __func__ << ": Activating window for " 
                << launchCommand);
        xWindows.activateWindow(window);
    }
    
}
