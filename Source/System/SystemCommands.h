#pragma once
#include "SharedResource_Handler.h"

/**
 * @file SystemCommands.h
 *
 * @brief  Accesses a read-only list of system commands.
 *
 * Pocket-home needs to be able to get information about the system it's running
 * on, and it needs to control several basic system properties.  These system
 * commands might not work on all systems, so rather than embedding them in the
 * program, SystemCommands reads them from a JSON file.
 *
 * All system commands are defined in the commands.json file in the pocket-home
 * data directory.  This file is stored as read-only rather than as a ConfigFile
 * because these commands should be kept secure and do not need to be changed
 * during execution.
 *
 * Command definitions should include a single executable name plus an optional
 * object list.  If more complex shell commands are needed, they should be
 * bundled into a shell script and placed in the scripts directory in the 
 * pocket-home data directory.  commands.json defines the full path to the
 * script directory with the "POCKET_HOME_SCRIPTS" key.  Any command text that
 * contains the literal string POCKET_HOME_SCRIPTS will have that string
 * replaced with the correct script directory path.
 *
 * All system commands are sorted into three categories.  ActionCommands
 * execute asynchronously, and return no data.  IntCommands return the command
 * process exit code.  TextCommands return all text output by the command
 * process.  TextCommands must always exit returning 0, or their text output
 * will be discarded.
 */

/* Private SharedResource class */
class SysCommandJSON;

class SystemCommands : public SharedResource::Handler<SysCommandJSON>
{
public:
    SystemCommands();
    
    virtual ~SystemCommands() { }

    /**
     * Lists all system commands that run asynchronously without returning
     * a value.
     */
    enum class ActionCommand
    {
        /* Shutdown the system */
        shutdown,
        /* Restart the system */
        restart,
        /* Enter sleep mode */
        sleep,
        /* Exit sleep mode */
        wake,
        /* Calibrate the touch screen */
        calibrate,
        /* Sets the system volume percentage */
        setVolume,
        /* Sets the display brightness percentage */
        setBrightness
    };

    /**
     * Lists all system commands that run until termination and return their
     * integer exit code.
     */
    enum class IntCommand
    {
        /* Returns 0 iff the system is sleeping */
        sleepCheck,
        /* Returns 0 iff a command is valid */
        commandCheck,
        /* Sets the application password, returning an error code on failure. */
        setPassword
    };

    /**
     * Lists all system commands that run until termination and return their
     * text output.
     */
    enum class TextCommand
    {
        /* Prints the system volume percentage */
        getVolume,
        /* Gets the battery percentage */
        getBatteryPercent,
        /* Gets the display brightness percentage */
        getBrightness
    };

    /**
     * @brief  Asynchronously runs a command in a new process.
     *
     * @param commandType  The system command to run.
     *
     * @param args         Arguments to pass to the command process.
     *
     * @return             True if the command was found and executed, false if
     *                     the command was not defined.
     */
    bool runActionCommand(const ActionCommand commandType,
            const juce::String& args = "");

    /**
     * @brief  Runs a command, waits for it to finish, and return its exit code.
     *
     * @param commandType  The system command to run.
     *
     * @param args         Arguments to pass to the command process.
     *
     * @return             The command process exit code, or -1 if the command
     *                     was not defined.
     */
    int runIntCommand(const IntCommand commandType,
            const juce::String& args = "");

    /**
     * @brief  Runs a command, waits for it to finish, and returns its text 
     *         output.
     *
     *  If the command process terminates with a nonzero exit code, no text will
     *  be returned.
     *
     * @param commandType  The system command to run.
     *
     * @param args         Arguments to pass to the command process.
     *
     * @return             All text printed by the command process, or the empty
     *                     string if the commandi was not defined, or did not 
     *                     terminate normally with an exit code of 0.
     */
    juce::String runTextCommand(const TextCommand commandType,
            const juce::String& args = "");

private:
    /**
     * @brief  Gets a system command string.
     *
     * @param commandKey  The key selecting the system command type.
     *
     * @param args        Any arguments that should be appended to the command.
     *
     * @return            The full command string, including arguments. 
     */
    juce::String getCommandString(const juce::Identifier& commandKey,
            const juce::String& args = "");
};
