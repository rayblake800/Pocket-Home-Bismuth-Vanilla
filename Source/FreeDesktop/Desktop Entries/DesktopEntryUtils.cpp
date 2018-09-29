#include <map>
#include <set>
#include "DesktopEntryFormatError.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryUtils.h"

/* Group header identifying the main section of desktop entry data. */
static const juce::Identifier mainGroupHeader("Desktop Entry");

/* String value at the start of action group headers. */
static const juce::String actionPrefix("Desktop Action ");

/* Characters that must be escaped if used in desktop entry files. */
static const juce::String charsToEscape("\"`$\\");

/* Characters that must be enclosed in double quotes */
static const juce::String reservedChars(" \t\n\"'\\><~|&;$*?#()`"); 

/*
 * Extracts the locale string from a line read from a desktop entry file.
 */
juce::String DesktopEntryUtils::parseLocale(const juce::String& line)
{
    using namespace juce;
    int openIndex = line.indexOfChar('[');
    if(openIndex > 0)
    {
        int closeIndex = line.indexOfChar(']');
        if(closeIndex > openIndex)
        {
            return line.substring(openIndex+1, closeIndex);
        }
    }
    return String();
}

/*
 * Extracts the data key from a line read from a desktop entry file.
 */
const juce::Identifier& DesktopEntryUtils::parseKey(const juce::String& line)
{
    using namespace juce;
    static const std::set<Identifier> keyList =
    {
        "Type",
        "Version",
        "Name",
        "GenericName",
        "NoDisplay",
        "Comment",
        "Icon",
        "Hidden",
        "OnlyShowIn",
        "NotShowIn",
        "DBusActivatable",
        "TryExec",
        "Exec",
        "Path",
        "Terminal",
        "Actions",
        "MimeType",
        "Categories",
        "Implements",
        "Keywords",
        "StartupNotify",
        "StartupWMClass",
        "URL"
    };

    if(!line.containsChar('='))
    {
        throw DesktopEntryFormatError(line);
    }
    String keyStr = line.initialSectionNotContaining("[=").trim();
    for(const Identifier& key : keyList)
    {
        if(key == StringRef(keyStr))
        {
            return key;
        }
    }
    throw DesktopEntryFormatError(keyStr);
}

/*
 * Extracts the data value from a line read from a desktop entry file.
 */
juce::String DesktopEntryUtils::parseValue(const juce::String& line)
{
    if(!line.containsChar('='))
    {
        throw DesktopEntryFormatError(line);
    }
    return line.fromLastOccurrenceOf("=", false, false);
}

/*
 * Makes a command execution string valid for writing into a desktop entry, by 
 * quoting all fields containing reserved characters, and escaping special 
 * characters when necessary.
 */
juce::String DesktopEntryUtils::quoteCommandFields(const juce::String& command)
{
    using namespace juce;
    StringArray commandSections = StringArray::fromTokens(command, true);
    for(String& section : commandSections)
    {
        if(section.containsAnyOf(reservedChars))
        {
            section = section.quoted();
            for(int i = 0; i < section.length(); i++)
            {
                if(charsToEscape.containsChar(section[i]))
                {
                    section = section.substring(0,i) 
                        + '\\'
                        + section.substring(i, section.length());
                    i++;
                }
            }
        }
    }
    return commandSections.joinIntoString(" ");
}

/*
 * Removes extra quotes and backslashes from a command execution string taken 
 * from a desktop entry file so that it is ready to have its field codes
 * expanded.
 */
juce::String DesktopEntryUtils::unquoteCommandFields
(const juce::String& command)
{
    using namespace juce;
    StringArray commandSections = StringArray::fromTokens(command, true);
    for(String& section : commandSections)
    {
        if(section.isQuotedString())
        {
            section = section.unquoted();
        }
        for(int i = section.length() - 1; i > 0; i--)
        {
            if(charsToEscape.containsChar(section[i]))
            {
                jassert(section[i - 1] == '\\');
                section = section.substring(0, i - 1)
                        + section.substring(i, section.length());
                i--;
            }
        }
    }
    return commandSections.joinIntoString(" ");
}

/*
 * Checks if a line from a desktop entry file contains a section header title.
 */
bool DesktopEntryUtils::isHeaderLine(const juce::String& line)
{
    return line.startsWithChar('[') && line.endsWithChar(']');
}

/*
 * Extracts a section header title from a desktop entry file line.
 */
juce::String DesktopEntryUtils::extractHeader(const juce::String& headerLine)
{
    return headerLine.substring(1, headerLine.length() - 1);
}

/*
 * Checks if a section header name describes the main desktop entry data 
 * section.
 */
bool DesktopEntryUtils::isMainDataHeader(const juce::String& header)
{
    return juce::StringRef(header) == mainGroupHeader;
}

/*
 * Checks if a section header name describes a desktop entry action.
 */
bool DesktopEntryUtils::isValidActionHeader(const juce::String& header)
{
    return header.startsWith(actionPrefix) 
        && header.length() > actionPrefix.length();
}
 
/*
 * Checks if a string is non-empty and contains only valid characters allowed in
 * desktop entry files.
 */
bool DesktopEntryUtils::isValidString(const juce::String& string,
        const bool isLocaleString)
{
    if(string.isEmpty())
    {
        return false;
    }
    for(int i = 0; i < string.length(); i++)
    {
        // TODO: Check if non-ascii characters are printable in the current
        //       locale.
        if((string[i] < ' ' || string[i] > '-')
          && !(isLocaleString && string[i] > 127))
        {
            return false;
        }
    }
    return true;
}

/*
 * Replaces newline, tab, carriage return, and backslash with the appropriate
 * escape character sequences.
 */
juce::String DesktopEntryUtils::addEscapeSequences
(const juce::String& rawString)
{
    using namespace juce;
    const std::map<char, String> escapeSequences =
        {
            {'\n' , "\\n"},
            {'\t' , "\\t"},
            {'\r' , "\\r"},
            {'\\' , "\\\\"}
        };
    String escaped = rawString;
    for(int i = rawString.length() - 1; i <=0; i--)
    {
        auto searchIter = escapeSequences.find(rawString[i]);
        if(searchIter != escapeSequences.end())
        {
            escaped = escaped.substring(0, i) + searchIter->second
                + escaped.substring(i+1, escaped.length());
        }
    }
    return escaped;
}

/*
 * Replaces \s, \n, \t, \r and \\ escape sequences with the characters they 
 * represent.
 */
juce::String DesktopEntryUtils::replaceEscapeSequences
(const juce::String& escapedString)
{
    using namespace juce;
    const std::map<char, char> escapeChars =
        {
            {'s', ' '},
            {'n', '\n'},
            {'t', '\t'},
            {'r', '\r'},
            {'\\', '\\'}
        };
    String rawString = escapedString;
    for(int i = 0; i < rawString.length(); i++)
    {
        if(rawString[i] == '\\')
        {
            if(i == rawString.length() - 1)
            {
                throw DesktopEntryFormatError(escapedString);
            }
            auto searchIter = escapeChars.find(rawString[i + 1]);
            if(searchIter == escapeChars.end())
            {
                throw DesktopEntryFormatError(escapedString);
            }
            rawString = rawString.substring(0, i) + searchIter->second
                + rawString.substring(i+2, rawString.length());
        }
    }
    return rawString;
}



/*
 * Validates a string value from a desktop entry file, and replaces its escape 
 * sequences with the appropriate character values.
 */
juce::String DesktopEntryUtils::processStringValue(
        const juce::String& entryString,
        const juce::File& sourceFile,
        const bool isLocaleString)
{
    using namespace juce;
    if(!isValidString(entryString, isLocaleString))
    {
        const String errorMessage = "Invalid string value";
        throw DesktopEntryFileError(sourceFile, errorMessage);
    }
    try
    {
        return replaceEscapeSequences(entryString);
    }
    catch(DesktopEntryFormatError e)
    {
        const String errorMessage = String("Invalid escape sequences in ")
            + e.what();
        throw DesktopEntryFileError(sourceFile, errorMessage);
    }
        
}

/*
 * Parses a list of strings from a single string value read from a desktop entry
 * file.
 */
juce::StringArray DesktopEntryUtils::parseList(
        const juce::String& listStr,
        const juce::File& sourceFile, 
        const bool isLocaleString)
{
    using namespace juce;
    String validatedStr = processStringValue
            (listStr, sourceFile, isLocaleString);
    StringArray list;
    if(listStr.containsChar(';'))
    {
        list.addTokens(listStr, ";", "\"");
    }
    else if(listStr.containsChar(','))
    {
        list.addTokens(listStr, ",", "\"");
    }
    else
    {
        list.add(listStr);
    }
    return list;
}

/*
 * Parses a boolean value from a string.
 */
bool DesktopEntryUtils::parseBool(const juce::String& boolStr,
        const juce::File& sourceFile)
{
    using namespace juce;
    // Standard boolean strings supported: true, false.
    // Using 0, 1 was once standard and should also be supported.
    if(boolStr == "true" || boolStr == "1")
    {
        return true;
    }
    else if(boolStr == "false" || boolStr == "0")
    {
        return false;
    }
    else
    {
        const String errorMessage = boolStr + " is not a valid boolean value!";
        throw DesktopEntryFileError(sourceFile, boolStr);
    }
}

/*
 * Converts a boolean value to a string that can be written to a desktop entry 
 * file.
 */
juce::String DesktopEntryUtils::boolString(const bool booleanValue)
{
    return booleanValue ? "true" : "false";
}

/*
 * Converts a list to a single string that can be written to a desktop entry
 * file.
 */
juce::String DesktopEntryUtils::listString(const juce::StringArray& list)
{
    for(juce::String& listItem : list)
    {
        listItem = addEscapeSequences(listItem);
        if(!isValidString(listItem))
        {
            throw DesktopEntryFormatError(listItem);
        }
    }
    return list.joinIntoString(";");
}

