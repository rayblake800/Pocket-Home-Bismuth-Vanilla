#include "ProcessUtils.h"
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
pid_t ProcessUtils::getProcessId()
{
    return getpid();
}

/**
 * Looks up information on a process using its process path.
 * 
 * @param processPath  The full path of a process directory in /proc
 * 
 * @return  Data describing the process, or an empty value if no process exists at
 *          the given path.
 */
static std::optional<ProcessUtils::ProcessData> getPathProcessData(juce::String processPath)
{
    using namespace juce;
    std::optional<ProcessUtils::ProcessData> matchingData;
    File statFile(processPath + "/stat");
    if(statFile.existsAsFile())
    {
        StringArray statItems 
                = StringArray::fromTokens(statFile.loadFileAsString(), true);
        ProcessUtils::ProcessData foundData;
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
 * Looks up information on a process using its process id.
 */
std::optional<ProcessUtils::ProcessData> ProcessUtils::getProcessData
(pid_t processId)
{
    using namespace juce;
    String path("/proc/");
    path += String(processId);
    return getPathProcessData(path); 
}

/*
 * Gets all processes that are direct child processes of a specific process.
 */
juce::Array<ProcessUtils::ProcessData> ProcessUtils::getChildProcesses
(pid_t processId)
{
    using namespace juce;
    File proc("/proc");
    Array<File> childDirs = proc.findChildFiles(File::findDirectories, false);
    Array<ProcessData> childProcs;
    for(const File& dir : childDirs)
    {
        std::optional<ProcessData> processData = getPathProcessData(dir.getFullPathName());
        if(processData && processData->parentId == processId)
        {
            childProcs.add(*processData);
        }
    }
    return childProcs;
}
