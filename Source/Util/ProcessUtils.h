#pragma once
#include <sys/types.h>
#include "JuceHeader.h"

/**
 * @file ProcessUtils.h
 * 
 * @brief Gets information on Linux processes.
 */
namespace ProcessUtils
{
    
    enum ProcessState
    {
        running,
        stopped,
        dead,
        sleep,
        uninterruptableSleep,
        nonexistent
    };
    
    struct ProcessData
    {
        int processId;
        int parentId;
        juce::String executableName;
        ProcessState lastState;    
        juce::uint64 startTime;
    };
    
    /**
     * Gets the id of the current process.
     * 
     * @return  The id of the process that is executing this function.
     */
    int getProcessId();
    
    /**
     * Looks up information on a process using its process id.
     * 
     * @param processId  The id of a Linux process.
     * 
     * @return  A data structure containing information about the process, one
     *          with a last state of ProcessState::nonexistent if no process is
     *          found.
     */
    ProcessData getProcessData(int processId);
    
    /**
     * Gets all processes that are direct child processes of a specific process.
     * 
     * @param processId  The id of a Linux process.
     * 
     * @return  An array of process data structures for each child that has
     *          processId as its parent id.  This array will be sorted with the
     *          newest elements listed first.
     */
    juce::Array<ProcessData> getChildProcesses(int processId);
}

