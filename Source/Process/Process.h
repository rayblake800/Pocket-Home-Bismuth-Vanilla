#pragma once
/**
 * @file  Process.h
 * 
 * @brief  Gets information about Linux processes.
 */

#include <sys/types.h>
#include "JuceHeader.h"

namespace Process
{
    /**
     * @brief  All possible Linux process states.
     */
    enum class State
    {          //Process state codes:
        running,     //R
        sleeping,    //S
        diskSleep,   //D
        zombie,      //Z
        stopped,     //T
        tracingStop, //t
        paging,      //W
        dead,        //X, x
        wakeKill,    //K
        parked,      //P
        idle,        //I
        unknown,
        invalid
    };
    
#ifdef JUCE_DEBUG
    /**
     * @brief  Gets a string representation of a process state for debugging.
     *
     * @param state  One of the possible Linux process states.
     *
     * @return       The process state's string representation. 
     */
    juce::String stateString(const State state);
#endif
    
    /**
     * @brief  Stores basic information about a single process.
     */
    struct Data
    {
        int processId;
        int parentId;
        juce::String executableName;
        State lastState;    
        juce::uint64 startTime;
    };
    
    /**
     * @brief  Gets the ID of the current process.
     * 
     * @return  The ID of the calling process.
     */
    int getProcessId();
    
    /**
     * @brief  Looks up information on a process using its process ID.
     * 
     * @param processId  The id of a Linux process.
     * 
     * @return           A data structure containing information about the 
     *                   process, or one with a last state of 
     *                   ProcessState::nonexistent if no process is found.
     */
    Data getProcessData(const int processId);
    
    /**
     * @brief  Gets data for all processes that are direct child processes of a
     *         specific process.
     * 
     * @param processId  The ID of a Linux process.
     * 
     * @return           An array of process data structures, containing Data
     *                   for each pracess that has processId as its parent ID.  
     *                   This array will be sorted with the newest elements 
     *                   listed first.
     */
    juce::Array<Data> getChildProcesses(const int processId);
}

