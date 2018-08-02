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
int ProcessUtils::getProcessId()
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
static ProcessUtils::ProcessData getPathProcessData(juce::String processPath)
{
    using namespace juce;
    ProcessUtils::ProcessData process =
    {
        .processId = -1,
        .parentId = -1,
        .executableName = "",
        .lastState = ProcessUtils::nonexistent
    };
    
    File statFile(processPath + "/stat");
    if(statFile.existsAsFile())
    {
        StringArray statItems 
                = StringArray::fromTokens(statFile.loadFileAsString(), true);
        process.processId = statItems[idIndex].getIntValue();
        process.executableName = statItems[nameIndex].removeCharacters("()");
        process.parentId = statItems[parentIdIndex].getIntValue();
        //juce Strings have no getUInt64 method, unfortunately
        process.startTime = 0;
        for(int i = 0; i < statItems[startTimeIndex].length(); i++)
        {
            process.startTime *= 10;
            process.startTime += (statItems[startTimeIndex][i] - '0');
        }
        char stateChar = statItems[stateIndex][0];
        switch(stateChar)
        {
            case 'R':
                process.lastState = ProcessUtils::running;
                break;
            case 'T':
            case 't':
            case 'W':
                process.lastState = ProcessUtils::stopped;
                break;
            case 'Z':
            case 'X':
            case 'x':
                process.lastState = ProcessUtils::dead;
                break;
            case 'S':
            case 'K':
                process.lastState = ProcessUtils::sleep;
                break;
            case 'D':
            case 'W':
            case 'P':
                process.lastState = ProcessUtils::uninterruptableSleep;
                break;
            default:
                DBG("ProcessUtils::" << __func__ 
                        << ": Unexpected process state " << stateChar);
                process.lastState = ProcessUtils::nonexistent;
                break;
        }
    }
    return process;   
}

/*
 * Looks up information on a process using its process id.
 */
ProcessUtils::ProcessData ProcessUtils::getProcessData
(int processId)
{
    using namespace juce;
    String path("/proc/");
    path += String(processId);
    return getPathProcessData(path); 
}

/**
 * Sorts processes by launch time, newest first.
 */
class
{
public:
    static int compareElements
    (ProcessUtils::ProcessData first, ProcessUtils::ProcessData second)
    {
        return second.startTime - first.startTime;
    }
    
} processComparator;


/*
 * Gets all processes that are direct child processes of a specific process.
 */
juce::Array<ProcessUtils::ProcessData> ProcessUtils::getChildProcesses
(int processId)
{
    using namespace juce;
    File proc("/proc");
    Array<File> childDirs = proc.findChildFiles(File::findDirectories, false);
    Array<ProcessData> childProcs;
    for(const File& dir : childDirs)
    {
        ProcessData processData = getPathProcessData(dir.getFullPathName());
        if(processData->parentId == processId)
        {
            childProcs.add(processData);
        }
    }
    childProcs.sort(processComparator);
    return childProcs;
}
