#pragma once
#include <exception>
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
     * @param entryFile       A valid desktop entry file.
     *
     * @throws FileException  If the file was not a valid desktop entry file.
     */
    DesktopEntry(const juce::File& entryFile);

    /**
     * @brief  Creates a desktop entry object without an existing file.
     *
     * @param name              The name of the new desktop entry. 
     *
     * @param filename          The name of the new entry file, without the file
     *                          extension.
     * 
     * @param type              The type of desktop entry to create.       
     *
     * @throws FormatException  If the name or filename provided do not comply
     *                          with desktop entry standards.
     *
     * @throws FileException    If the file already exists.
     */
    DesktopEntry(const juce::String& name, const juce::String& filename,
            const Type type);
            
    virtual ~DesktopEntry() { }
    
    /**
     * @brief  Signals that a desktop entry file read from disk is invalid.
     */
    struct FileException : public std::exception
    {
    public:
        /**
         * @brief  Creates a new exception for an invalid desktop entry file.
         *
         * @param file   The invalid file being read.
         *
         * @param error  A brief description of the problem encountered.
         */
        FileException(juce::File& file, juce::String& error) :
            errorMessage(file.getFullPathName() + ": " + error) { }

        /**
         * @brief  Gets a string describing the error.
         *
         * @return  A string containing the file's path and a short description
         *          of the file error.
         */
        virtual const char* what() const noexcept override
        {
            return errorMessage.toRawUTF8();
        }
    private:
        juce::String errorMessage;
    };
    
    /**
     * @brief  Signals that an attempt was made to set a value that is invalid
     *         for the desktop entry format.
     */
    struct FormatException : public std::exception
    {
    public:
        /**
         * @brief Creates a new exception for an invalid data value.
         *
         * @param badValue  The value that failed to comply with desktop entry
         *                  standards.
         */
        FormatException(const juce::String& badValue) :
        badValue(badValue),
        errorMessage(juce::String("Invalid desktop entry value: ") + badValue)
        { }

        /**
         * @brief  Gets a string describing the error. 
         *
         * @return  A non-specific format error message, along with the invalid
         *          value that triggered the exception.
         */
        virtual const char* what() const noexcept override
        {
            return errorMessage.toRawUTF8();
        }

        /**
         * @brief  Gets the value that triggered the exception.
         *
         * @return  The value that failed to comply with desktop entry 
         *          standards.
         */
        const juce::String& getBadValue() const
        {
            return badValue;
        }

    private:
        const juce::String badValue;
        const juce::String errorMessage;
    };

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
     * @param name              The new desktop entry title to apply.
     *
     * @throws FormatException  If the name does not comply with desktop
     *                          entry standards.
     */
    void setName(const juce::String& name);

    /**
     * @brief  Sets the generic name describing the entry.
     *
     * @param name              The new generic name to apply.
     *
     * @throws FormatException  If the generic name does not comply with desktop
     *                          entry standards.
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
     * @param icon              The new icon name or path value.
     *
     * @throws FormatException  If the icon value does not comply with desktop
     *                          entry standards.
     */
    void setIcon(const juce::String& icon);

    /**
     * @brief  Sets the string value used to construct the launch command.
     *
     * @param newExec           The new string value used as the base launch
     *                          command.  Arguments will be quoted if necessary.
     *
     * @throws FormatException  If the value does not comply with desktop
     *                          entry standards.
     */
    void setExec(const juce::String& newExec);

    /**
     * @brief  Sets the application name used when checking if this application
     *         is valid.
     *
     * @param newTryExec        The name or path of an executable to test.
     *
     * @throws FormatException  If the value does not comply with desktop
     *                          entry standards.
     */
    void setTryExec(const juce::String& newTryExec);

    /**
     * @brief  Sets the path where this application should run.
     *
     * @param runningDirectory  The new working directory path to use.
     *
     * @throws FormatException  If the path string does not comply with desktop
     *                          entry standards.
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
     * @param newCategories     The new application category list to apply.
     *
     * @throws FormatException  If any categories do not comply with desktop
     *                          entry standards.
     */
    void setCategories(const juce::StringArray& newCategories);

    /**
     * @brief  Sets the list of keywords associated with this desktop entry.
     *
     * @param newKeywords       The new keyword list to apply.
     *
     * @throws FormatException  If any keywords do not comply with desktop
     *                          entry standards.
     */
    void setKeywords(const juce::StringArray& newKeywords);

    /**
     * @brief  Writes this desktop entry to the user's local application data
     *         directory.
     *
     * @throws FileException  If necessary field values were not defined, or the
     *                        file could not be written for any other reason.
     */
    void writeFile();

private:
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

    /* Utility functions for processing desktop entry file data: */

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

    LineData parseLine(const juce::String& line);
    /**
     * @brief  Saves data from a desktop entry line to the appropriate 
     *         DesktopEntry fields.
     *
     * @param lineData        Data from a line under the "[Desktop Entry]" 
     *                        header.
     *
     * @throws FileException  If a line contained invalid data.
     */
    void saveLineData(const LineData& lineData);

    /**
     * @brief  Saves data from a desktop entry line to the most recently created
     *         desktop action.
     *
     * @param lineData  Data from a line under a desktop action header.
     *
     * @throws FileException  If a line contained invalid data.
     */
    void saveActionLineData(const LineData& lineData);

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
     * @brief  Checks if a line from a desktop entry file contains a section
     *         header title.
     *
     * @param line  The line read from the .desktop or .directory file.
     *
     * @return  True if the line contains a valid header title.
     */
    static bool isHeaderLine(const juce::String& line);

    /**
     * @brief  Extracts a section header title from a desktop entry file line.
     *
     * @param headerLine  A line read from a desktop entry file, that has
     *                    already been confirmed to be a header line using 
     *                    isHeaderLine().
     *
     * @return            The header title, assuming the line is a valid header
     *                    line.  Return values for invalid header lines are
     *                    undefined.
     */
    static juce::String extractHeader(const juce::String& headerLine);

    /**
     * @brief  Checks if a section header name describes the main desktop entry
     *         data section.
     *
     * @param header  A section header name read from a desktop entry file.
     *
     * @return   Whether the header name exactly matches the standard name for
     *           the main data section.
     */
    static bool isMainDataHeader(const juce::String& header);

    /**
     * @brief  Checks if a section header name describes a desktop entry action.
     *
     * @param header  A section header name read from a desktop entry file.
     *
     * @return   Whether the header name matches the format of desktop entry
     *           action data.
     */
    static bool isValidActionHeader(const juce::String& header);
    
    /**
     * @brief  Checks if a string is non-empty and contains only valid 
     *         characters allowed in desktop entry files.
     *
     * @param string  The string to search for invalid characters.
     *
     * @return        Whether the string is non-empty and all characters in the 
     *                string are valid.
     */
    static bool isValidString(const juce::String& string);
    
    /**
     * @brief  Parses a list of strings from a single string value read from
     *         a desktop entry file.
     *
     * @param listStr         A semicolon-separated list of string values. 
     *                        Comma-separated lists are no longer standard, but 
     *                        these will also be accepted, to support files 
     *                        created before the current desktop entry standard.
     *
     * @throws FileException  If the list contained invalid characters.
     *
     * @return                The list of string values.
     */
    static juce::StringArray parseList(const juce::String& listStr);

    /**
     * @brief  Parses a boolean value from a string.
     *
     * @param boolStr         A boolean value, either "true" or "false".  Values
     *                        of "1" or "0" will also be accepted, to support 
     *                        files created before the current desktop entry
     *                        standard.  If any other values are encountered, 
     *                        this object will be marked as invalid.
     *
     * @throws FileException  If the string was not a valid boolean value.
     *
     * @return                The boolean value represented by the string.
     */
    static bool parseBool(const juce::String& boolStr);

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
    
    /* If, when reading a desktop entry, invalid data is encountered, this is
       set to false to indicate that the entry fails to meet specifications and
       should be ignored. */
    bool isValid = true;
};

