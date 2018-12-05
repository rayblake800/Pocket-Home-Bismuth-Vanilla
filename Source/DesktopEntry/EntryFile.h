#pragma once
#include "JuceHeader.h"
#include <map>

/* 
 * @file  DesktopEntry/EntryFile.h
 * 
 * @brief  Reads in standardized .desktop application shortcut files.
 *
 * Desktop entry files store all the information needed to create application
 * shortcuts to display in menus.  Each EntryFile loads and shares the data from
 * one of these files, or collects the data needed to create a new desktop entry
 * file.
 *
 * EntryFile objects follow the desktop entry standard defined at 
 * freedesktop.org, ignoring invalid data when reading files, and preventing
 * invalid data from being added to desktop entry files. Invalid actions are
 * prevented through the use of DesktopEntry::FormatError exceptions.
 * 
 * Although .directory files are part of the desktop entry standard, they are
 * not relevant to this module and will be ignored.
 */
namespace DesktopEntry { class EntryFile; }

class DesktopEntry::EntryFile
{
public:
    /**
     * @brief  Defines all supported types of desktop entry.
     */
    enum class Type
    {
        application,
        link,
    };

    /**
     * @brief  Loads desktop entry data from a .desktop file.
     * 
     * @param  sourceFile     A valid desktop entry file.
     *
     * @param  desktopFileID  The entry file's desktop file ID.
     *
     * @throws FileError      If the file was not a valid desktop entry file.
     */
    EntryFile(const juce::File& sourceFile, const juce::String& desktopFileID);

    /**
     * @brief  Creates a desktop entry object without an existing file.
     *
     * @param name           The name of the new desktop entry. 
     *
     * @param desktopFileID  The new entry's desktop file ID.
     * 
     * @param type           The type of desktop entry to create.       
     *
     * @throws FormatError   If the name or filename provided do not comply with
     *                       desktop entry standards.
     */
    EntryFile(const juce::String& name, const juce::String& desktopFileID,
            const Type type);

    /**
     * @brief  Creates an empty desktop entry object with no data.
     */
    EntryFile() { }
            
    virtual ~EntryFile() { }

    /**
     * @brief  Checks if the EntryFile does not contain all data required by
     *         a valid desktop entry.
     *
     * @return  Whether the desktop entry is missing required data fields.
     */
    bool isMissingData() const;

    /**
     * @brief Checks if two desktop entries have the same desktop file ID.
     *
     * @param toCompare  Another desktop entry to compare with this one.
     *
     * @return           True if and only if both entries have identical desktop
     *                   file IDs.
     */
    bool operator==(const EntryFile& toCompare) const;
    
    /**
     * @brief Alphabetically compares entries based on their names.
     *
     * @param toCompare  Another desktop entry to compare with this one.
     *
     * @return  True if this entry's name comes before toCompare's name
     *          alphabetically, false otherwise.
     */
    bool operator<(const EntryFile& toCompare) const;

    /**
     * @brief  Gets the desktop entry's type.
     *
     * @return  The entry type.
     */
    Type getType() const;

    /**
     * @brief  Gets the unique desktop file ID that identifies this entry.
     *
     * @return  The entry's desktop file ID.
     */
    juce::String getDesktopFileID() const;

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
     *         this entry's application is valid.
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
     * @brief  Checks if this entry's application should be launched in a new 
     *         terminal window.
     *
     * @return   Whether the application should be launched within a new 
     *           terminal window.
     */
    bool getLaunchedInTerm() const;

    /**
     * @brief  Gets the IDs of all alternate actions supported by this desktop
     *         entry.
     *
     * @return  The list of action IDs.
     */
    juce::StringArray getActionIDs() const;

    /**
     * @brief  Gets the display title of a desktop entry action.
     *
     * @param actionID  The string identifying a desktop entry action.
     *
     * @return          The action's title, or the empty string if the action
     *                  was not found.
     */
    juce::String getActionTitle(const juce::String actionID) const;

    /**
     * @brief  Gets the name or path of an action's icon.
     *
     * @param actionID  The string identifying a desktop entry action.
     *
     * @return          The action's icon name or path, or the empty string if
     *                  the index is invalid or the action has no icon.
     */
    juce::String getActionIcon(const juce::String actionID) const;

    /**
     * @brief  Gets the command used to run a desktop entry action.
     *
     * @param actionID  The string identifying a desktop entry action.
     *
     * @return       The action's launch command, or the empty string if the
     *               index is invalid.
     */
    juce::String getActionLaunchCommand(const juce::String actionID) const;

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

    /**
     * @brief  Sets the desktop entry's name
     *
     * @param name          The new desktop entry title to apply.
     *
     * @throws FormatError  If the name does not comply with desktop entry 
     *                      standards.
     */
    void setName(const juce::String& name);

    /**
     * @brief  Sets the generic name describing the entry.
     *
     * @param name          The new generic name to apply.
     *
     * @throws FormatError  If the generic name does not comply with desktop 
     *                      entry standards.
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
     * @param icon          The new icon name or path value.
     *
     * @throws FormatError  If the icon value does not comply with desktop entry
     *                      standards.
     */
    void setIcon(const juce::String& icon);

    /**
     * @brief  Sets the string value used to construct the entry's launch 
     *         command.
     *
     * @param newExec       The new string value used as the base launch 
     *                      command.  Arguments will be quoted if necessary.
     *
     * @throws FormatError  If the value does not comply with desktop entry 
     *                      standards.
     */
    void setExec(const juce::String& newExec);

    /**
     * @brief  Sets the application name used when checking if this entry's
     *         application is valid.
     *
     * @param newTryExec    The name or path of an executable to test.
     *
     * @throws FormatError  If the value does not comply with desktop entry
     *                      standards.
     */
    void setTryExec(const juce::String& newTryExec);

    /**
     * @brief  Sets the path where this entry's application should run.
     *
     * @param runningDirectory  The new working directory path to use.
     *
     * @throws FormatError      If the path string does not comply with desktop 
     *                          entry standards.
     */
    void setRunDirectory(const juce::String& runningDirectory);

    /**
     * @brief  Sets if this entry's application should be launched in a new 
     *         terminal window.
     *
     * @param termLaunch  If true, the application will launch in a new terminal
     *                    window.  If false, the application will launch 
     *                    normally.
     */
    void setLaunchedInTerm(const bool termLaunch);

    /**
     * @brief  Sets the list of categories associated with this desktop entry.
     *
     * @param newCategories  The new application category list to apply.
     *
     * @throws FormatError   If any categories do not comply with desktop entry 
     *                       standards.
     */
    void setCategories(const juce::StringArray& newCategories);

    /**
     * @brief  Sets the list of keywords associated with this desktop entry.
     *
     * @param newKeywords   The new keyword list to apply.
     *
     * @throws FormatError  If any keywords do not comply with desktop entry 
     *                      standards.
     */
    void setKeywords(const juce::StringArray& newKeywords);

    /**
     * @brief  Writes this desktop entry to the user's local application data
     *         directory.
     *
     * @throws FileError  If necessary field values were not defined, or the 
     *                    file could not be written for any other reason.
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
     * @throws FileError  If the file doesn't exist or contains invalid data.
     */
    void readEntryFile();

    /**
     * @brief  Saves data from a desktop entry key/value pair to the appropriate 
     *         EntryFile fields.
     *
     * @param key         The key read from the desktop entry line.
     *
     * @param value       The value read from the desktop entry line.
     *
     * @throws FileError  If the key or value were invalid.
     */
    void saveLineData(const juce::Identifier& key, const juce::String& value);

    /**
     * @brief  Saves data from a desktop entry line to the most recently created
     *         desktop action.
     *
     * @param actionID  The desktop action's ID string
     *
     * @param key       The key read from the desktop entry line.
     *
     * @param value     The value read from the desktop entry line.
     */
    void saveActionLineData(const juce::String actionID, 
            const juce::Identifier& key, const juce::String& value);

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
     * @brief  For a single data key, stores how to read that key's EntryFile 
     *         data from a .desktop file, and how to convert that EntryFile 
     *         data back into .desktop file data.
     */
    struct DataConverter
    {
        /* Stores a desktop entry file value in an EntryFile */
        const std::function<void(EntryFile*,const juce::String&)> readValue;
        /* Reads a desktop entry value from an EntryFile */
        const std::function<juce::String(EntryFile*)> getValue;
    };
    
    /* Stores all data keys defined in the desktop entry specifications,
       mapped to functions for importing and exporting that key's data.  */
    static const std::map<juce::Identifier, DataConverter> keyGuide;

    /* The source .desktop file. */
    juce::File file;

    /* The desktop entry's desktop file ID. */
    juce::String desktopFileID;
    
    /* The desktop entry's type. */
    Type type = Type::application;

    /* Specific name of the entry's application. */
    juce::String name;

    /* Generic name of the entry's application. */
    juce::String genericName;

    /* Sets if the entry should appear in menus */
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

    /* Whether this entry's program runs in a terminal window. */
    bool terminal = false;

    /* An alternate action the entry can perform. */
    struct Action
    {
        /* The action's title */
        juce::String title;
        /* The action's icon name or path. */
        juce::String icon;
        /* The command to execute the action. */
        juce::String exec;
    };

    /* Names identifying application actions. */
    juce::StringArray actionTypes;
    
    /* Maps Action ID strings to Action data. */
    std::map<juce::String, Action> actions;

    /* MIME types supported by this entry's application. */
    juce::StringArray mimeTypes;

    /* Categories in which this entry should be shown. */
    juce::StringArray categories;

    /* A list of interfaces that this entry's application implements. */
    juce::StringArray implements;

    /* A list of strings describing the entry. */
    juce::StringArray keywords;

    /* Whether the entry's application is known to support startup notification
     */
    bool startupNotify;

    /* If specified, it is known that the entry's application will map to a 
       window with this string as its WM class or name hint. */
    juce::String startupWMClass;

    /* The URL used if this entry is a link. */
    juce::String url;
};

