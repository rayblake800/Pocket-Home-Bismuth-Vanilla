#pragma once
#include <sys/types.h>
#include <optional>
#include "JuceHeader.h"

/**
 * @file LinuxProcess.h
 * 
 * @brief Gets information on Linux processes.
 */
namespace LinuxProcess
{
    
    enum ProcessState
    {
        running,
        stopped,
        sleep,
        uninterruptableSleep
    };
    
    struct ProcessData
    {
        pid_t processId;
        pid_t parentId;
        juce::String executableName;
        ProcessState lastState;    
        juce::uint64 startTime;
    };
    
    /**
     * Gets the id of the current process.
     * 
     * @return  The id of the process that is executing this function.
     */
    pid_t getProcessId();
    
    /**
     * Looks up information on a process using its process id.
     * 
     * @param processId  The id of a Linux process.
     * 
     * @return  A data structure containing information about the process, or an
     *          empty value if no matching process is found.
     */
    std::optional<ProcessData> getProcessData(pid_t processId);
    
    /**
     * Gets all processes that are direct child processes of a specific process.
     * 
     * @param processId  The id of a Linux process.
     * 
     * @return  An array of process data structures for each child that has
     *          processId as its parent id.
     */
    juce::Array<ProcessData> getChildProcesses(pid_t processId);
}

