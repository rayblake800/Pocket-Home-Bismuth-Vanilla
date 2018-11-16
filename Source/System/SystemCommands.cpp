#include "JSONFile.h"
#include "LaunchedProcess.h"
#include "SharedResource/Resource.h"
#include "SystemCommands.h"

/* Script directory key */
static const juce::Identifier scriptDirKey("POCKET_HOME_SCRIPTS");

/* SharedResource object key */
static const juce::Identifier jsonResourceKey("SysCommandJSON");

/* JSON resource file name */
static const juce::String filename("commands.json");


/*########################   Action Commands:   ##############################*/
/* Shutdown the system */
static const juce::Identifier shutdownKey("shutdown");
/* Restart the system */
static const juce::Identifier restartKey("restart");
/* Enter sleep mode */
static const juce::Identifier sleepKey("sleep");
/* Exit sleep mode */
static const juce::Identifier wakeKey("wake");
/* Calibrate the touch screen */
static const juce::Identifier calibrateKey("calibrate");
/* Sets the system volume percentage */
static const juce::Identifier setVolumeKey("set volume");
/* Sets the display brightness percentage */
static const juce::Identifier setBrightnessKey("set brightness");

/**
 * @brief Gets the Identifier key used to store an ActionCommand.
 *
 * @param commandType  The requested system command.
 *
 * @return             The key needed to load the command from the JSON file.
 */
static const juce::Identifier& actionCommandKey
(const SystemCommands::ActionCommand commandType)
{
    using ActionCommand = SystemCommands::ActionCommand;
    switch(commandType)
    {
        case ActionCommand::shutdown:
            return shutdownKey;
        case ActionCommand::restart:
            return restartKey;
        case ActionCommand::sleep:
            return sleepKey;
        case ActionCommand::wake:
            return wakeKey;
        case ActionCommand::calibrate:
            return calibrateKey;
        case ActionCommand::setVolume:
            return setVolumeKey;
        case ActionCommand::setBrightness:
            return setBrightnessKey;
    }
    DBG("SystemCommand::" << __func__ << ":Error, unhandled command type!");
    jassertfalse;
    return juce::Identifier::null;
}

/*########################    Int Commands:     ##############################*/
/* Returns 0 iff the system is sleeping */
static const juce::Identifier sleepCheckKey("sleep check");
/* Returns 0 iff a command is valid */
static const juce::Identifier commandTestKey("command check");
/* Sets the application password, returning an error code on failure. */
static const juce::Identifier setPasswordKey("set password");

/**
 * @brief Gets the Identifier key used to store an IntCommand.
 *
 * @param commandType  The requested system command.
 *
 * @return             The key needed to load the command from the JSON file.
 */
static const juce::Identifier& intCommandKey
(const SystemCommands::IntCommand commandType)
{
    using IntCommand = SystemCommands::IntCommand;
    switch(commandType)
    {
        case IntCommand::sleepCheck:
            return sleepCheckKey;
        case IntCommand::commandCheck:
            return commandTestKey;
        case IntCommand::setPassword:
            return setPasswordKey;
    }
    DBG("SystemCommand::" << __func__ << ":Error, unhandled command type!");
    jassertfalse;
    return juce::Identifier::null;
}

/*########################    Text Commands:    ##############################*/
/* Prints the system volume percentage */
static const juce::Identifier getVolumeKey("get volume");
/* Gets the battery percentage */
static const juce::Identifier getBatteryKey("get battery percent");
/* Gets the display brightness percentage */
static const juce::Identifier getBrightnessKey("get brightness");

/**
 * @brief Gets the Identifier key used to store a TextCommand.
 *
 * @param commandType  The requested system command.
 *
 * @return             The key needed to load the command from the JSON file.
 */
static const juce::Identifier& textCommandKey
(const SystemCommands::TextCommand commandType)
{
    using TextCommand = SystemCommands::TextCommand;
    switch(commandType)
    {
        case TextCommand::getVolume:
           return getVolumeKey;
        case TextCommand::getBatteryPercent:
           return getBatteryKey;
        case TextCommand::getBrightness:
             return getBrightnessKey;
    }
    DBG("SystemCommand::" << __func__ << ":Error, unhandled command type!");
    jassertfalse;
    return juce::Identifier::null;
}


/* Private SharedResource class */
class SysCommandJSON : public SharedResource::Resource, public JSONFile
{
public:
    friend class SystemCommands;
    SysCommandJSON() : SharedResource::Resource(jsonResourceKey), 
    JSONFile(filename) 
    {
        using namespace juce;
        scriptDir = getProperty<String>(scriptDirKey);
        if(!File(scriptDir).exists())
        {
            DBG("SysCommandJSON: " << scriptDir <<
                    " is not a valid script directory.");
            scriptDir = String();
        }
    }

    virtual ~SysCommandJSON() { }

private:
    /* Pocket-Home script directory path: */
    juce::String scriptDir;
};

SystemCommands::SystemCommands() :
SharedResource::Handler<SysCommandJSON>(jsonResourceKey) { }


/*
 * Asynchronously runs a command in a new process.
 */
bool SystemCommands::runActionCommand(const ActionCommand commandType,
        const juce::String& args)
{
    using namespace juce;
    String command = getCommandString(actionCommandKey(commandType), args);
    if(command.isEmpty())
    {
        return false;
    }
    juce::Thread::launch([command]() { system(command.toRawUTF8()); });
    return true;
}

/*
 * Runs a command, waits for it to finish, and return its exit code.
 */
int SystemCommands::runIntCommand(const IntCommand commandType,
        const juce::String& args)
{
    using namespace juce;
    String command = getCommandString(intCommandKey(commandType), args);
    if(command.isEmpty())
    {
        return -1;
    }
    return system(command.toRawUTF8());
}

/*
 * Runs a command, waits for it to finish, and returns its text output.
 */
juce::String SystemCommands::runTextCommand(const TextCommand commandType,
        const juce::String& args)
{
    using namespace juce;
    String command = getCommandString(textCommandKey(commandType), args);
    if(command.isEmpty())
    {
        return String();
    }
    LaunchedProcess commandProcess(command);
    commandProcess.waitForProcessToFinish();
    return commandProcess.getProcessOutput();
}

/*
 * Gets a system command string.
 */
juce::String SystemCommands::getCommandString
(const juce::Identifier& commandKey, const juce::String& args)
{   
    using namespace juce;
    auto json = getReadLockedResource();
    String command = json->getProperty<String>(commandKey); 
    if(command.isEmpty())
    {
        DBG("SystemCommands::" << __func__ << ": No command defined for key "
                << commandKey.toString());
        return String();
    }
    if(json->scriptDir.isNotEmpty())
    {
        String dirKeyString = scriptDirKey.toString();
        int index = command.indexOf(dirKeyString);
        if(index != -1)
        {
            command = command.substring(0, index) + json->scriptDir
                + command.substring(index + dirKeyString.length());
        }
    }
    if(args.isNotEmpty())
    {
        command += " ";
        command += args;
    }
    return command;
}


