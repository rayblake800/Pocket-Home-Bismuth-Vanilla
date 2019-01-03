#ifndef DESKTOP_ENTRY_IMPLEMENTATION
  #error File included outside of DesktopEntry implementation.
#endif

#pragma once
#include "JuceHeader.h"

/**
 * @file  DesktopEntry/FileUtils.h
 *
 * @brief  Helps DesktopEntry validate, process, and create desktop entry data.
 */
namespace DesktopEntry
{
    namespace FileUtils
    {    
        /**
         * @brief  Extracts the locale string from a line read from a desktop 
         *         entry file.
         *
         * @param line  A desktop entry line that may target a specific locale.
         *
         * @return      The extracted locale string, or the empty string if the 
         *              line does not target a specific locale.
         */
        juce::String parseLocale(const juce::String& line);

        /**
         * @brief  Extracts the data key from a line read from a desktop entry 
         *         file.
         *
         * @param line          A line from a desktop entry containing a 
         *                      key/value pair.
         *
         * @throws FormatError  If the line does not contain a valid key.
         *
         * @return              The extracted key string.
         */
        const juce::Identifier& parseKey(const juce::String& line);

        /**
         * @brief  Extracts the data value from a line read from a desktop entry
         *         file.
         *
         * @param line          A line from a desktop entry containing a 
         *                      key/value pair.
         *
         * @throws FormatError  If the line does not contain a valid value.
         *
         * @return              The extracted value string.
         */
        juce::String parseValue(const juce::String& line);

        /**
         * @brief  Makes a command execution string valid for writing into a 
         *         desktop entry. 
         *
         * This quotes all fields containing reserved characters, and escapes 
         * special characters when necessary.
         *
         * @param command  An application launch command, possibly containing
         *                 arguments and desktop entry field codes.
         *
         * @return         A version of that command that can be written to a 
         *                 desktop entry file.
         */
        juce::String quoteCommandFields(const juce::String& command);

        /**
         * @brief  Removes extra quotes and backslashes from a command execution
         *         string taken from a desktop entry file so that it is ready to
         *         have its field codes expanded.
         *
         * @param command  The desktop entry's execution command string.
         *
         * @return         A version of the command string ready to be processed
         *                 with expandFieldCodes.
         */
        juce::String unquoteCommandFields(const juce::String& command);

        /**
         * @brief  Checks if a line from a desktop entry file contains a section
         *         header title.
         *
         * @param line  The line read from the .desktop or .directory file.
         *
         * @return      True if the line contains a valid header title.
         */
        bool isHeaderLine(const juce::String& line);

        /**
         * @brief  Extracts a section header title from a desktop entry file 
         *         line.
         *
         * @param headerLine  A line read from a desktop entry file, that has
         *                    already been confirmed to be a header line using 
         *                    isHeaderLine().
         *
         * @return            The header title, assuming the line is a valid 
         *                    header line.  Return values for invalid header 
         *                    lines are undefined.
         */
        juce::String extractHeader(const juce::String& headerLine);

        /**
         * @brief  Checks if a section header name describes the main desktop 
         *         entry data section.
         *
         * @param header  A section header name read from a desktop entry file.
         *
         * @return        Whether the header name exactly matches the standard 
         *                name for the main data section.
         */
        bool isMainDataHeader(const juce::String& header);

        /**
         * @brief  Gets the section header used to identify the main data 
         *         section of a desktop entry.
         *
         * @return  The main data header, "[Desktop Entry]".
         */
        juce::String getMainHeader();

        /**
         * @brief  Checks if a section header name describes a desktop entry 
         *         action.
         *
         * @param header  A section header name read from a desktop entry file.
         *
         * @return        Whether the header name matches the format of desktop 
         *                entry action data.
         */
        bool isValidActionHeader(const juce::String& header);

        /**
         * @brief  Extracts only the action ID from a section header returned by
         *         the extractHeader function.
         *
         * @param extractedHeader  An action header string returned by 
         *                         extractHeader().
         *
         * @return                 The action's string ID, or the empty string 
         *                         if extractedHeader wasn't a valid extracted 
         *                         icon header.
         */
        juce::String extractActionID(const juce::String& extractedHeader);
        
        /**
         * @brief  Checks if a string contains only valid characters allowed in 
         *         desktop entry files.
         *
         * @param string          The string to search for invalid characters.
         *
         * @param isLocaleString  Whether the string contains localized text 
         *                        that has a larger set of permitted characters.
         *
         * @return                Whether all characters in the string are 
         *                        valid.
         */
        bool isValidString
        (const juce::String& string, const bool isLocaleString);

        /**
         * @brief  Replaces newline, tab, carriage return, and backslash
         *         with the appropriate escape character sequences.
         *
         * @param rawString  A string that needs to be written to a desktop 
         *                   entry file.
         *
         * @return           A copy of the string parameter, with characters 
         *                   replaced by escape sequences.
         */
        juce::String addEscapeSequences(const juce::String& rawString);

        /**
         * @brief  Replaces \s, \n, \t, \r and \\ escape sequences with the
         *         characters they represent.
         *
         * @param escapedString  A string read from a desktop entry file.
         *
         * @throws FormatError   If the string contains invalid escape 
         *                       sequences.
         *
         * @return               A copy of the string parameter, with escape 
         *                       sequences replaced by their character values.
         */
        juce::String replaceEscapeSequences(const juce::String& escapedString);

        /**
         * @brief  Validates a string value from a desktop entry file, and 
         *         replaces its escape sequences with the appropriate character
         *         values.
         *
         * @param entryString     A string value from a desktop entry file.
         *
         * @param sourceFile      The desktop entry file where the string was 
         *                        found.
         *
         * @param isLocaleString  Whether the string value contains localized 
         *                        text that may use non-ascii characters.
         *
         * @throws FileError      If the string value read from the file was not
         *                        compliant with desktop entry specifications.
         *
         * @return               The processed string value.
         */
        juce::String processStringValue(const juce::String& entryString,
                const juce::File& sourceFile, const bool isLocaleString);

        /**
         * @brief  Parses a list of strings from a single string value read from
         *         a desktop entry file.
         *
         * @param listStr         A semicolon-separated list of string values. 
         *                        Comma-separated lists are no longer standard, 
         *                        but will also be accepted to support files 
         *                        created before the current desktop entry 
         *                        standard.
         *
         * @param sourceFile      The desktop entry file where the string was 
         *                        found.
         *
         * @param isLocaleString  Whether the string value contains localized 
         *                        text that may use non-ascii characters.
         *
         * @throws FileError      If the list string value read from the file 
         *                        was not compliant with desktop entry 
         *                        specifications.
         *
         * @return                The list of string values.
         */
        juce::StringArray parseList(const juce::String& listStr,
                const juce::File& sourceFile, const bool isLocaleString);

        /**
         * @brief  Parses a boolean value from a string.
         *
         * @param boolStr     A boolean value, either "true" or "false". Values 
         *                    of "1" or "0" will also be accepted, to support 
         *                    files created before the current desktop entry 
         *                    standard.
         *
         * @param sourceFile  The desktop entry file where the string was found.
         *
         * @throws FileError  If the string was not a valid boolean value.
         *
         * @return            The boolean value represented by the string.
         */
        bool parseBool(const juce::String& boolStr, const juce::File& sourceFile);

        /**
         * @brief  Converts a boolean value to a string that can be written to a
         *         desktop entry file.
         *
         * @param booleanValue  The boolean value to convert.
         *
         * @return              The value's string representation.
         */
        juce::String boolString(const bool booleanValue);

        /**
         * @brief  Converts a list to a single string that can be written to a 
         *         desktop entry file.
         *
         * @param list            The list to convert.
         *
         * @throws FormatError    If any list strings contain invalid 
         *                        characters.
         *
         * @param isLocaleString  Whether the string value contains localized 
         *                        text that may use non-ascii characters.
         *
         * @return                The list's string representation.
         */
        juce::String listString(const juce::StringArray& list,
                const bool isLocaleString);
    }
}

