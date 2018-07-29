#include "LinuxProcess.h"
#include <unistd.h>

//Indices of process data members within the process stat file
static const constexpr int idIndex        = 0;
static const constexpr int nameIndex      = 1;
static const constexpr int stateIndex     = 2;
static const constexpr int parentIdIndex  = 3;
static const constexpr int startTimeIndex = 21;

/*
 * Gets the id of the current process.
 */
pid_t LinuxProcess::getProcessId()
{
    return getpid();
}

/*
 * Looks up information on a process using its process id.
 */
std::optional<ProcessData> LinuxProcess::getProcessData(pid_t processId)
{
    using namespace juce;
    std::optional<ProcessData> matchingData;
    File statFile(String("/proc/") + String(processId) + "/stat");
    if(statFile.existsAsFile())
    {
        StringArray statItems 
                = StringArray::fromTokens(statFile.loadFileAsString(), true);
        ProcessData foundData;
        foundData.processId = statItems[idIndex].getIntValue();
        foundData.executableName = statItems[nameIndex];
        foundData.parentId = statItems[parentIdIndex].getIntValue();
        //juce Strings have no getUInt64 method, unfortunately
        foundData.startTime = 0;
        for(int i = 0; i < statItems[startTimeIndex].length(); i++)
        {
            foundData.startTime *= 10;
            foundData.startTime += (statItems[startTimeIndex][i] - '0');
        }
        matchingData = foundData;
    }
    return matchingData;   
}

/*
 * Gets all processes that are direct child processes of a specific process.
 */
juce::Array<ProcessData> LinuxProcess::getChildProcesses(pid_t processId);
