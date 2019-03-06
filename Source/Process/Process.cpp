#include "Process.h"
#include <unistd.h>

//Indices of process data members within the process stat file
static const constexpr int idIndex        = 0;
static const constexpr int nameIndex      = 1;
static const constexpr int stateIndex     = 2;
static const constexpr int parentIdIndex  = 3;
static const constexpr int startTimeIndex = 21;

#ifdef JUCE_DEBUG

/*
 * Gets a string representation of a process state for debugging.
 */
juce::String Process::stateString(const Process::State state)
{
    switch(state)
    {
        case State::running:
            return "running";
        case State::sleeping:
            return "sleeping";
        case State::diskSleep:
            return "diskSleep";
        case State::zombie:
            return "zombie";
        case State::stopped:
            return "stopped";
        case State::tracingStop:
            return "tracingStop";
        case State::paging:
            return "paging";
        case State::dead:
            return "dead";
        case State::wakeKill:
            return "wakeKill";
        case State::parked:
            return "parked";
        case State::idle:
            return "idle";
        case State::unknown:
            return "unknown";
        case State::invalid:
            return "invalid";
    }
    return "[unhandled State!]";
}
#endif

/*
 * Gets the ID of the current process.
 */
int Process::getProcessId()
{
    return getpid();
}

/**
 * @brief Looks up information on a process using its process path.
 * 
 * @param processPath  The full path of a process directory in /proc
 * 
 * @return  Data describing the process, or an empty value if no process exists at
 *          the given path.
 */
static Process::Data getPathData(juce::String processPath)
{
    using namespace Process;
    Data process =
    {
        .processId = -1,
        .parentId = -1,
        .executableName = "",
        .lastState = State::invalid
    };
    
    juce::File statFile(processPath + "/stat");
    if(statFile.existsAsFile())
    {
        // Parse process info from stat file strings:
        juce::StringArray statItems = juce::StringArray::fromTokens(
                statFile.loadFileAsString(), true);
        process.processId = statItems[idIndex].getIntValue();
        process.executableName = statItems[nameIndex].removeCharacters("()");
        process.parentId = statItems[parentIdIndex].getIntValue();

        //juce::String has no getUInt64 method, unfortunately
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
            case 'C':
                process.lastState = State::running;
                break;
            case 'S':
                process.lastState = State::sleeping;
                break;
            case 'D':
                process.lastState = State::diskSleep;
                break;
            case 'Z':
                process.lastState = State::zombie;
                break;
            case 'T':
                process.lastState = State::stopped;
                break;
            case 't':
                process.lastState = State::tracingStop;
                break;
            case 'W':
                process.lastState = State::paging;
                break;
            case 'X':
            case 'x':
                process.lastState = State::dead;
                break;
            case 'K':
                process.lastState = State::wakeKill;
                break;
            case 'P':
                process.lastState = State::parked;
                break;
            default:
                process.lastState = State::unknown;
        }
    }
    return process;   
}

/*
 * Looks up information on a process using its process ID.
 */
Process::Data Process::getProcessData(const int processId)
{
    juce::String path("/proc/");
    path += juce::String(processId);
    return getPathData(path); 
}

/*
 * Sorts processes by launch time, newest first.
 */
class
{
public:
    static int compareElements(Process::Data first, Process::Data second)
    {
        return second.startTime - first.startTime;
    }
    
} processComparator;


/*
 * Gets all processes that are direct child processes of a specific process.
 */
juce::Array<Process::Data> Process::getChildProcesses(const int processId)
{
    using juce::File;
    using juce::Array;
    File proc("/proc");
    Array<File> childDirs = proc.findChildFiles(File::findDirectories, false);
    Array<Data> childProcs;
    for(const File& dir : childDirs)
    {
        Data processData = getPathData(dir.getFullPathName());
        if(processData.parentId == processId)
        {
            childProcs.add(processData);
        }
    }
    childProcs.sort(processComparator);
    return childProcs;
}
