#pragma once
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
        Array(ProcessUtils::ProcessData> childProcs
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
 * Destroys the LaunchedApp object. This does not stop the associated child
 * process.
 */
LaunchedApp::~LaunchedApp()
{
}

/*
 * Gets the launch command used to start the process.
 */
juce::String LaunchedApp::getLaunchCommand()
{
}

/*
 * Checks if the launched application is still running.
 */
bool LaunchedApp::isRunning()
{
}

/*
 * Attempts to terminate the application process.
 */
bool LaunchedApp::kill()
{
}

/*
 * Waits for the application process to end.
 */
void LaunchedApp::waitForProcessToFinish(int timeoutMs)
{
}

/*
 * Gets all text output by the process to stdout and stderr.
 */
juce::String LaunchedApp::getProcessOutput()
{
}

/*
 * Moves the application's windows in front of all other windows and focuses
 * them.
 */
void LaunchedApp::activateWindow()
{
}
