#pragma once
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
     * @brief  Defines all types of desktop entry.
     */
    enum class Type
    {
        application,
        link,
        directory
    };

    /**
     * @brief  Loads desktop entry data from a .desktop or .directory file.
     * 
     * @param entryFile               A valid desktop entry file.
     *
     * @throws DesktopEntryFileError  If the file was not a valid desktop entry
     *                                file.
     */
    DesktopEntry(const juce::File& entryFile);

    /**
     * @brief  Creates a desktop entry object without an existing file.
     *
     * @param name                      The name of the new desktop entry. 
     *
     * @param filename                  The name of the new entry file, without
     *                                  the file extension.
     * 
     * @param type                      The type of desktop entry to create.       
     *
     * @throws DesktopEntryFormatError  If the name or filename provided do not 
     *                                  comply with desktop entry standards.
     */
    DesktopEntry(const juce::String& name, const juce::String& filename,
            const Type type);
            
    virtual ~DesktopEntry() { }

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

    /* ########## Functions for getting desktop entry data: ################# */

    /**
     * @brief  Gets the desktop entry's type.
     *
     * @return  The entry type.
     */
    Type getType() const;

    /**
     * @brief  Gets the desktop entry's name.
     *
     * @return  The title of this desktop entry.
     */
    juce::String getName() const;

    /**
     * @brief  Gets a generic name describing the entry.
     *
     * @return  A generic name value, e.g. "Web Browser".
     */
    juce::String getGenericName() const;

    /**
     * @brief  Checks if this desktop entry should appear in application menus.
     *
     * @return  True if the entry should be shown, false if it should be hidden.
     */
    bool shouldBeDisplayed() const;

    /**
     * @brief  Gets the name or path of the desktop entry's icon.
     *
     * @return  The icon name or path.
     */
    juce::String getIcon() const;

    /**
     * @brief  Gets the command string used to launch this entry's application.
     *
     * @return  The launch command string, with arguments unquoted and field
     *          values expanded as necessary.
     */
    juce::String getLaunchCommand() const;

    /**
     * @brief  Gets the string value used to construct the launch command.
     *
     * @return  An unprocessed desktop entry string value.
     */
    juce::String getExec() const;

    /**
     * @brief  Gets an application name or path to check when determining if
     *         this application is valid.
     *
     * @return  The executable's path, or the name of an executable that can be
     *          found through the PATH environment variable.
     */
    juce::String getTryExec() const;

    /**
     * @brief  Gets the path where this application should run.
     *
     * @return  The application's working directory.
     */
    juce::String getRunDirectory() const;

    /**
     * @brief  Checks if this application should be launched in a new terminal
     *         window.
     *
     * @return   True if and only if the application should be launched within
     *           a new terminal window.
     */
    bool getLaunchedInTerm() const;

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
     * @brief  Gets a list of keywords associated with this desktop entry.
     *
     * @return  The entry's keyword list.
     */
    juce::StringArray getKeywords() const;

    /* ########## Functions for setting desktop entry data: ################# */
    
    /**
     * @brief  Sets the desktop entry's name
     *
     * @param name                      The new desktop entry title to apply.
     *
     * @throws DesktopEntryFormatError  If the name does not comply with desktop
     *                                  entry standards.
     */
    void setName(const juce::String& name);

    /**
     * @brief  Sets the generic name describing the entry.
     *
     * @param name                      The new generic name to apply.
     *
     * @throws DesktopEntryFormatError  If the generic name does not comply with
     *                                  desktop entry standards.
     */
    void setGenericName(const juce::String& name);

    /**
     * @brief  Sets if this desktop entry should appear in application menus.
     *
     * @param showEntry  True to show the entry, false to hide it.
     */
    void setIfDisplayed(const bool showEntry);

    /**
     * @brief Sets the name or path of the desktop entry's icon.
     *
     * @param icon                      The new icon name or path value.
     *
     * @throws DesktopEntryFormatError  If the icon value does not comply with 
     *                                  desktop entry standards.
     */
    void setIcon(const juce::String& icon);

    /**
     * @brief  Sets the string value used to construct the launch command.
     *
     * @param newExec                   The new string value used as the base 
     *                                  launch command.  Arguments will be 
     *                                  quoted if necessary.
     *
     * @throws DesktopEntryFormatError  If the value does not comply with 
     *                                  desktop entry standards.
     */
    void setExec(const juce::String& newExec);

    /**
     * @brief  Sets the application name used when checking if this application
     *         is valid.
     *
     * @param newTryExec                The name or path of an executable to 
     *                                  test.
     *
     * @throws DesktopEntryFormatError  If the value does not comply with
     *                                  desktop entry standards.
     */
    void setTryExec(const juce::String& newTryExec);

    /**
     * @brief  Sets the path where this application should run.
     *
     * @param runningDirectory          The new working directory path to use.
     *
     * @throws DesktopEntryFormatError  If the path string does not comply with 
     *                                  desktop entry standards.
     */
    void setRunDirectory(const juce::String& runningDirectory);

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
     * @brief  Sets the list of categories associated with this desktop entry.
     *
     * @param newCategories             The new application category list to 
     *                                  apply.
     *
     * @throws DesktopEntryFormatError  If any categories do not comply with
     *                                  desktop entry standards.
     */
    void setCategories(const juce::StringArray& newCategories);

    /**
     * @brief  Sets the list of keywords associated with this desktop entry.
     *
     * @param newKeywords               The new keyword list to apply.
     *
     * @throws DesktopEntryFormatError  If any keywords do not comply with
     *                                  desktop entry standards.
     */
    void setKeywords(const juce::StringArray& newKeywords);

    /**
     * @brief  Writes this desktop entry to the user's local application data
     *         directory.
     *
     * @throws DesktopEntryFileError  If necessary field values were not 
     *                                defined, or the file could not be written
     *                                for any other reason.
     */
    void writeFile();

private:
    /**
     * @brief  Given a standard desktop entry data key, get the value
     *         mapped to that key.
     *
     * @param key  A key string defined in the Desktop Entry specifications.
     *
     * @return     The corresponding value, encoded as a String that may be
     *             written to a desktop entry file.
     */
    juce::String getValue(const juce::Identifier& key);

    /**
     * @brief  Loads all desktop entry data from the desktop entry's file.
     *
     * @throws DesktopEntryFileError  If the file doesn't exist or contains 
     *                                invalid data.
     */
    void readEntryFile();

    /**
     * @brief  Saves data from a desktop entry key/value pair to the appropriate 
     *         DesktopEntry fields.
     *
     * @param key                     The key read from the desktop entry line.
     *
     * @param value                   The value read from the desktop entry 
     *                                line.
     *
     * @throws DesktopEntryFileError  If the key or value were invalid.
     */
    void saveLineData(const juce::Identifier& key, const juce::String& value);

    /**
     * @brief  Saves data from a desktop entry line to the most recently created
     *         desktop action.
     *
     * @param key                     The key read from the desktop entry line.
     *
     * @param value                   The value read from the desktop entry 
     *                                line.
     *
     * @throws DesktopEntryFileError  If the key or value were invalid.
     */
    void saveActionLineData
    (const juce::Identifier& key, const juce::String& value);

    /**
     * @brief  Expands all field codes in a command string, removing them and
     *         replacing them with appropriate values.
     *
     * @param execString  A command string to expand.
     *
     * @return            The command string with desktop entry field codes
     *                    expanded, or the empty string if invalid header codes
     *                    were found.
     */
    juce::String expandFieldCodes(const juce::String& execString) const;

    /**
     * @brief  For a single data key, stores how to read that key's DesktopEntry 
     *         data from a .desktop file, and how to convert that DesktopEntry 
     *         data back into .desktop file data.
     */
    struct DataConverter
    {
        /* Stores a desktop entry file value in a DesktopEntry */
        const std::function<void(DesktopEntry*,const juce::String&)> readValue;
        /* Reads a desktop entry value from a DesktopEntry */
        const std::function<juce::String(DesktopEntry*)> getValue;
    };
    
    /* Stores all data keys defined in the desktop entry specifications,
       mapped to functions for importing and exporting that key's data.  */
    static const std::map<juce::Identifier, DataConverter> keyGuide;

    /* The source .desktop or .directory file. */
    juce::File entryFile;

    /* The desktop entry's desktop file ID. */
    juce::String desktopFileID;
    
    /* The desktop entry's type. */
    Type type = Type::application;

    /* Specific name of the application. */
    juce::String name;

    /* Generic name of the application. */
    juce::String genericName;

    /* Sets if the application should appear in menus */
    bool noDisplay = false;

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
    bool terminal = false;

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

    /* Names identifying application actions. */
    juce::StringArray actionTypes;

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

