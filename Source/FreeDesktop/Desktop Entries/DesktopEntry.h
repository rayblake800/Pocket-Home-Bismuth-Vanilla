#pragma once
#include <map>
#include "AppConfigFile.h"

/* 
 * @file DesktopEntry.h
 * 
 * DesktopEntry reads in standardized .Desktop file data
 * the Desktop entry format is very well documented at
 * https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html
 */

class DesktopEntry
{
public:
    /**
     * @brief  Loads desktop entry data from a .desktop or .directory file.
     * 
     * @param entryFile  A valid desktop entry file.
     */
    DesktopEntry(juce::File entryFile);

    /**
     * @brief  Creates an empty desktop entry object.
     *
     * @param name  The name of the new desktop entry. 
     */
    DesktopEntry(juce::String name);
            
    virtual ~DesktopEntry() { }
    
    /**
     * @brief  Defines all types of desktop entry.
     */
    enum class Type
    {
        application,
        link,
        directory
    };

    /**
     * @brief  Gets the desktop entry's type.
     *
     * @return  The entry type.
     */
    Type getType() const;

    /**
     * @brief  Sets the desktop entry's type.
     *
     * @param type  The new entry type.
     */
    void setType(const Type type);

    /**
     * @brief  Gets the desktop entry's name.
     *
     * @return  The title of this desktop entry.
     */
    juce::String getName() const;

    /**
     * @brief  Sets the desktop entry's name
     *
     * @param name  The new desktop entry title to apply.
     */
    void setName(const juce::String& name);

    /**
     * @brief  Gets a generic name describing the entry.
     *
     * @return  A generic name value, e.g. "Web Browser".
     */
    juce::String getGenericName() const;

    /**
     * @brief  Sets the generic name describing the entry.
     *
     * @param name  The new generic name to apply.
     */
    void setGenericName(const juce::String& name);

    /**
     * @brief  Checks if this desktop entry should appear in application menus.
     *
     * @return  True if the entry should be shown, false if it should be hidden.
     */
    bool shouldBeDisplayed() const;

    /**
     * @brief  Sets if this desktop entry should appear in application menus.
     *
     * @param showEntry  True to show the entry, false to hide it.
     */
    void setIfDisplayed(const bool showEntry);

    /**
     * @brief  Gets the name or path of the desktop entry's icon.
     *
     * @return  The icon name or path.
     */
    juce::String getIconName() const;

    /**
     * @brief Sets the name or path of the desktop entry's icon.
     *
     * @param icon  The new icon name or path value.
     */
    void setIconName(const juce::String& icon);

    /**
     * @brief  Gets the command string used to launch this entry's application.
     *
     * @return  The launch command string.
     */
    juce::String getLaunchCommand() const;

    /**
     * @brief  Gets the string value used to construct the launch command.
     *
     * @return  An unprocessed desktop entry string.
     */
    juce::String getExecString() const;

    /**
     * @brief  Sets the string value used to contstruct the launch command.
     *
     * @param newExec  The new string value used as the base launch command.
     */
    void setExecString(const juce::String& newExec);

    /**
     * @brief  Gets an application name or path to check when determining if
     *         this application is valid.
     *
     * @return  The executable's path, or the name of an executable that can be
     *          found through the PATH environment variable.
     */
    juce::String getTryExec() const;

    /**
     * @brief  Sets the application name used when checking if this application
     *         is valid.
     *
     * @param newTryExec  The name or path of an executable to test.
     */
    void setTryExec(const juce::String& newTryExec);

    /**
     * @brief  Gets the path where this application should run.
     *
     * @return  The application's working directory.
     */
    juce::String getRunDirectory() const;

    /**
     * @brief  Sets the path where this application should run.
     *
     * @param runningDirectory  The new working directory path to use.
     */
    void setRunDirectory(const juce::String& runningDirectory);

    /**
     * @brief  Checks if this application should be launched in a new terminal
     *         window.
     *
     * @return   True if and only if the application should be launched within
     *           a new terminal window.
     */
    bool getLaunchedInTerm() const;

    /**
     * @brief  Sets if this application should be launched in a new terminal
     *         window.
     *
     * @param termLaunch  If true, the application will launch in a new terminal
     *                    window.  If false, the application will launch 
     *                    normally.
     */
    void setLaunchedInTerm(const bool termLaunch);

    /**
     * @brief  Gets the names of all alternate actions supported by this desktop
     *         entry.
     *
     * @return  The list of action names.
     */
    juce::StringArray getActionNames() const;

    /**
     * @brief  Gets the name or path of an action's icon.
     *
     * @param index  The index of an action, which should match the index of
     *               the action's name in the list returned by getActionNames().
     *
     * @return       The action's icon name or path, or the empty string if the
     *               index is invalid or the action has no icon.
     */
    juce::String getActionIcon(const int index) const;

    /**
     * @brief  Gets the command used to run a desktop entry action.
     *
     * @param index  The index of an action, which should match the index of
     *               the action's name in the list returned by getActionNames().
     *
     * @return       The action's launch command, or the empty string if the
     *               index is invalid.
     */
    juce::String getActionLaunchCommand(const int index) const;

    /**
     * @brief  Gets the list of categories associated with this desktop entry.
     *
     * @return  The list of all application category strings.
     */
    juce::StringArray getCategories() const;

    /**
     * @brief  Sets the list of categories associated with this desktop entry.
     *
     * @param newCategories  The new application category list to apply.
     */
    void setCategories(const juce::StringArray& newCategories);

    /**
     * @brief  Gets a list of keywords associated with this desktop entry.
     *
     * @return  The entry's keyword list.
     */
    juce::StringArray getKeywords() const;

    /**
     * @brief  Sets the list of keywords associated with this desktop entry.
     *
     * @param newKeywords  The new keyword list to apply.
     */
    void setKeywords(const juce::StringArray& newKeywords);

    /**
     * @brief  Writes this desktop entry to the user's local application data
     *         directory.
     *
     * @return  True if the entry was written, false if the entry contained
     *          invalid or missing data.
     */
    bool writeFile();

    /**
     * @brief Checks if two desktop entries have the same desktop file ID.
     *
     * @param toCompare  Another desktop entry to compare with this one.
     *
     * @return           True if and only if both entries have identical desktop
     *                   file IDs.
     */
    bool operator==(const DesktopEntry& toCompare) const;
    
    /**
     * @brief Alphabetically compares entries based on their names.
     *
     * @param toCompare  Another desktop entry to compare with this one.
     *
     * @return  True if this entry's name comes before toCompare's name
     *          alphabetically, false otherwise.
     */
    bool operator<(const DesktopEntry& toCompare) const;

private:
    /**
     * @brief  Checks if this desktop entry's data follows the desktop entry
     *         standard.
     *
     * @return  True if all data matches the standard, false if required fields
     *          are missing or field values are invalid.
     */
    bool isDataValid() const;

    /**
     * @brief Data read from a single desktop entry line.
     */
    struct LineData
    {
        juce::String locale;
        juce::String key;
        juce::String value;
    };

    /**
     * @brief  Reads the locale, key, and value from a single line in a desktop
     *         entry file.
     *
     * @param line  A line from a desktop entry file.
     *
     * @return   The data requested from the line.
     */
    static LineData parseLine(const juce::String& line);

    /**
     * @brief  Expands all field codes in a command string, removing them and
     *         replacing them with appropriate values.
     *
     * @param execString  A command string to expand.
     *
     * @return            The command string with desktop entry field codes
     *                    expanded, or the empty string if execString contained
     *                    invalid field codes.
     */
    juce::String expandFieldCodes(const juce::String& execString) const;

    /**
     * @brief  Saves data from a desktop entry line to the appropriate 
     *         DesktopEntry fields.
     *
     * @param lineData  Data from a line under the "[Desktop Entry]" header.
     */
    void saveLineData(const LineData& lineData);

    /**
     * @brief  Saves data from a desktop entry line to the most recently created
     *         desktop action.
     *
     * @param lineData  Data from a line under a desktop action header.
     */
    void saveActionLineData(const LineData& lineData);

    /**
     * @brief  Given a standard desktop entry data key, get the value
     *         mapped to that key.
     *
     * @param key  A key string defined in the Desktop Entry specifications.
     *
     * @return  The corresponding value, encoded as a String that may be written
     *          to a desktop entry file.
     */
    juce::String getValue(const juce::Identifier& key);

    /**
     * @brief  Gets the desktop file ID of the desktop entry.
     *
     * @return  The file ID, constructed from the file's name and path.
     */
    juce::String getDesktopFileId() const;

    /* The source .desktop or .directory file. */
    juce::File entryFile;
    
    /* The desktop entry's type. */
    Type type;

    /* Specific name of the application. */
    juce::String name;

    /* Generic name of the application. */
    juce::String genericName;

    /* Sets if the application should appear in menus */
    bool noDisplay;

    /* Tooltip describing the entry. */
    juce::String comment;

    /* The entry icon's name or path. */
    juce::String icon;

    /* If not empty, defines the only desktop environments that should show the
       entry. */
    juce::StringArray onlyShowIn;

    /* The list of desktop environments where the entry should not appear. */
    juce::StringArray notShowIn;

    /* Sets if this entry is an application that should be activated over
       D-Bus */
    bool dBusActivatable = false;

    /* Path to an executable file on disk used to determine if the program is
       actually installed. */
    juce::String tryExec;

    /* Program to execute, possibly with arguments. */
    juce::String exec;

    /* If the entry is an application, the working directory where the program
       should run. */
    juce::String path;

    /* Whether this program runs in a terminal window. */
    bool terminal;

    /* An alternate action the entry can perform. */
    struct Action
    {
        /* The action's title */
        juce::String name;
        /* The action's icon name or path. */
        juce::String icon;
        /* The command to execute the action. */
        juce::String exec;
    };

    /* All alternate actions the entry can perform. */
    juce::Array<Action> actions;

    /* MIME types supported by this application. */
    juce::StringArray mimeTypes;

    /* Categories in which this entry should be shown. */
    juce::StringArray categories;

    /* A list of interfaces that this application implements. */
    juce::StringArray implements;

    /* A list of strings describing the entry. */
    juce::StringArray keywords;

    /* Whether the application is known to support startup notification */
    bool startupNotify;

    /* If specified, it is known that the application will map to a window
       with this string as its WM class or name hint. */
    juce::String startupWMClass;

    /* The URL used if this entry is a link. */
    juce::String url;
};


