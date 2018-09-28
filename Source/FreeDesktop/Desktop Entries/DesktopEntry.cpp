#include <set>
#include "Localized.h"
#include "Utils.h"
#include "MainConfigFile.h"
#include "MainConfigKeys.h"
#include "XDGDirectories.h"
#include "DesktopEntry.h"

/* Group header identifying the main section of desktop entry data. */
static const juce::Identifier mainGroupHeader("Desktop Entry");

/* String value at the start of action group headers. */
static const juce::String actionPrefix("Desktop Action ");

/* Command used to activate applications over D-Bus. */
static const juce::String dBusActivateCommand 
        = "dbus-launch --exit-with-session ";

/* Characters that must be escaped if used in desktop entry files. */
static const constexpr char* charsToEscape = "\"`$\\";

/* Characters that must be enclosed in double quotes */
static const constexpr char* reservedChars
        = "\s\t\n\"'\\><~|&;$*?#()`"; 

/* Directory where desktop entries are stored within $XDG_DATA_DIRS */
static const juce::String entryDirectory = "applications/";

/* All data keys defined in the desktop entry specifications. */
static const juce::Array<juce::Identifier> keys =
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


/*
 * Loads desktop entry data from a .desktop or .directory file.
 */
DesktopEntry::DesktopEntry(const juce::File& entryFile) :
entryFile(entryFile)
{
    using namespace juce;
    StringArray lines;
    entryFile.readLines(lines);
    String locale = Localized::getLocaleName();
    String groupHeader;
    for (const String& line : lines)
    {
        if(!isValid) // Don't continue after finding invalid data.
        {
            break;
        }
        if(line.startsWithChar('#'))
        {
            continue; //skip comments
        }
        else if(isHeaderLine(line))
        {
            groupHeader = extractHeader(line);
            if(isValidActionHeader(groupHeader))
            {
                actions.add(Action());
            }
            else if(!isMainDataHeader(groupHeader))
            {
                DBG("DesktopEntry::DesktopEntry: Invalid group header "
                        << groupHeader);
                isValid = false;
            }
        }
        LineData lineData = parseLine(line);
        if (lineData.key.isNotEmpty() &&
            (lineData.locale.isEmpty() || lineData.locale == locale))
        {
            if(mainGroupHeader == StringRef(groupHeader))
            {
                saveLineData(lineData);
            }
            else
            {
                saveActionLineData(lineData);
            }
        }
    }
}


/*
 *  Creates a desktop entry object without an existing file.
 */
DesktopEntry::DesktopEntry
(const juce::String& name, const juce::String& filename, const Type type) : 
name(name),
entryFile(XDGDirectories::getUserDataPath() 
        + entryDirectory + name + ".desktop") 
{ }
          
/*
 * Checks if two desktop entries have the same desktop file ID.
 */
bool DesktopEntry::operator==(const DesktopEntry& toCompare) const
{
    return getDesktopFileId() == toCompare.getDesktopFileId();
}
    
/*
 * Alphabetically compares entries based on their names.
 */
bool DesktopEntry::operator<(const DesktopEntry& toCompare) const
{
    return name < toCompare.name;
}

/* ############## Functions for getting desktop entry data: ################# */

/*
 * Gets the desktop entry's type.
 */
DesktopEntry::Type DesktopEntry::getType() const
{
    return type;
}

/*
 * Gets the desktop entry's name.
 */
juce::String DesktopEntry::getName() const
{
    return name;
}

/*
 * Gets a generic name describing the entry.
 */
juce::String DesktopEntry::getGenericName() const
{
    return genericName;
}

/*
 * Checks if this desktop entry should appear in application menus.
 */
bool DesktopEntry::shouldBeDisplayed() const
{
    return !noDisplay && onlyShowIn.isEmpty();
}

/*
 * Gets the name or path of the desktop entry's icon.
 */
juce::String DesktopEntry::getIcon() const
{
    return icon;
}

/*
 * Gets the command string used to launch this entry's application.
 */
juce::String DesktopEntry::getLaunchCommand() const
{
    using namespace juce;
    String command = expandFieldCodes(exec);
    if(terminal && command.isNotEmpty())
    {
        MainConfigFile config;
        command = config.getConfigValue<String>
                (MainConfigKeys::termLaunchCommandKey) + " " + command;
    }
    return command;
}

/*
 * Gets the string value used to construct the launch command.
 */
juce::String DesktopEntry::getExec() const
{
    return exec;
}

/*
 * Gets an application name or path to check when determining if this 
 * application is valid.
 */
juce::String DesktopEntry::getTryExec() const
{
    return tryExec;
}

/*
 * Gets the path where this application should run.
 */
juce::String DesktopEntry::getRunDirectory() const
{
    return path;
}

/*
 * Checks if this application should be launched in a new terminal window.
 */
bool DesktopEntry::getLaunchedInTerm() const
{
    return terminal;
}

/*
 * Gets the names of all alternate actions supported by this desktop entry.
 */
juce::StringArray DesktopEntry::getActionNames() const
{
    using namespace juce;
    StringArray names;
    for(const Action& action : actions)
    {
        names.add(action.name);
    }
    return names;
}

/*
 * Gets the name or path of an action's icon.
 */
juce::String DesktopEntry::getActionIcon(const int index) const
{
    if(index < 0 || index >= actions.size())
    {
        return juce::String();
    }
    return actions[index].icon;
}

/*
 * Gets the command used to run a desktop entry action.
 */
juce::String DesktopEntry::getActionLaunchCommand(const int index) const
{
    if(index < 0 || index >= actions.size())
    {
        return juce::String();
    }
    return expandFieldCodes(actions[index].exec);
}

/*
 * Gets the list of categories associated with this desktop entry.
 */
juce::StringArray DesktopEntry::getCategories() const
{
    return categories;
}

/*
 * Gets a list of keywords associated with this desktop entry.
 */
juce::StringArray DesktopEntry::getKeywords() const
{
    return keywords;
}

/* ############## Functions for setting desktop entry data: ################# */

/*
 * Sets the desktop entry's name.
 */
void DesktopEntry::setName(const juce::String& newName)
{
    name = newName;
}

/*
 * Sets the generic name describing the entry.
 */
void DesktopEntry::setGenericName(const juce::String& newGenericName)
{
    genericName = newGenericName;
}

/*
 * Sets if this desktop entry should appear in application menus.
 */
void DesktopEntry::setIfDisplayed(const bool showEntry)
{
    noDisplay = !showEntry;
}

/*
 * Sets the name or path of the desktop entry's icon.
 */
void DesktopEntry::setIcon(const juce::String& newIcon)
{
    icon = newIcon;
}

/*
 * Sets the string value used to construct the launch command.
 */
void DesktopEntry::setExec(const juce::String& newExec)
{
    exec = newExec;
}

/*
 * Sets the application name used when checking if this application is valid.
 */
void DesktopEntry::setTryExec(const juce::String& newTryExec)
{
    tryExec = newTryExec;
}

/*
 * Sets the path where this application should run.
 */
void DesktopEntry::setRunDirectory(const juce::String& runningDirectory)
{
    path = runningDirectory;
}

/*
 * Sets if this application should be launched in a new terminal window.
 */
void DesktopEntry::setLaunchedInTerm(const bool termLaunch)
{
    terminal = termLaunch;
}

/*
 * Sets the list of categories associated with this desktop entry.
 */
void DesktopEntry::setCategories(const juce::StringArray& newCategories)
{
    categories = newCategories;
}

/*
 * Sets the list of keywords associated with this desktop entry.
 */
void DesktopEntry::setKeywords(const juce::StringArray& newKeywords)
{
    keywords = newKeywords;
}

/*
 * Writes this desktop entry to the user's local application data directory.
 */
void DesktopEntry::writeFile()
{
    using namespace juce;
    String outFileText = "";
    String locale = Localized::getLocaleName();
    StringArray foundKeys;
    // Reload the source file to preserve comments and alternate locale data.
    StringArray lines;
    entryFile.readLines(lines);
    String sectionHeader;
    for(const String& line : lines)
    {
        if(outFileText.isNotEmpty())
        {
            outFileText += "\n";
        }
        // Find and copy section headers
        if(line.startsWithChar('['))
        {
            sectionHeader = line.substring(1, line.length() - 1);
        }
        // Parse lines when under the main header, and the line is not a
        // comment.
        else if(StringRef(sectionHeader) == mainGroupHeader
                && !line.startsWithChar('#'))
        {
            LineData lineData = parseLine(line);
            if (lineData.key.isNotEmpty()
                    && (lineData.locale.isEmpty() 
                    || lineData.locale == locale))
            {
                foundKeys.add(lineData.key);
                outFileText += lineData.key;
                if (lineData.locale.isNotEmpty())
                {
                    outFileText += "[";
                    outFileText += lineData.locale;
                    outFileText += "]";
                }
                outFileText += "=";
                outFileText += getValue(lineData.key);
                continue;
            }
        }
        // Copy field headers, unexpected lines, comments, Actions, and data 
        // from other locales unedited.
        outFileText += line;
    }
    // Copy keys not found in the original file
    for (const Identifier& key : keys)
    {
        if (!foundKeys.contains(key.toString()))
        {
            String value = getValue(key);
            if(value.isNotEmpty())
            {
                outFileText += String("\n") + key + String("=") + value;
            }
        }

    }
    // Write files to the user data directory:
    File outFile(XDGDirectories::getUserDataPath()
            + entryDirectory
            + entryFile.getFullPathName().fromLastOccurrenceOf
                    (entryDirectory, false, false));
    outFile.create();
    DBG("DesktopEntry::" << __func__ << ": writing file " 
            << outFile.getFullPathName()); 
    outFile.replaceWithText(outFileText);
}

/*
 * Given a standard desktop entry data key, get the value mapped to that key.
 */
juce::String DesktopEntry::getValue(const juce::Identifier& key)
{
}

/*
 * Gets the desktop file ID of the desktop entry.
 */
juce::String DesktopEntry::getDesktopFileId() const
{
}

/*
 * Reads the locale, key, and value from a single line in a desktop entry file.
 */
DesktopEntry::LineData DesktopEntry::parseLine(const juce::String& line)
{
    LineData lineData;
    lineData.key = line.initialSectionNotContaining("[=");
    if(lineData.key.isEmpty())
    {
        // line is not a valid data line, return empty line data.
        return lineData;
    }
    int openIndex = line.indexOfChar('[');
    if(openIndex > 0)
    {
        int closeIndex = line.indexOfChar(']');
        if(closeIndex < 0)
        {
            // Desktop entry contains an unclosed square bracket.
            // Something is wrong, return empty line data.
            isValid = false;
            return lineData;
        }
        lineData.locale = line.substring(openIndex+1, closeIndex);
    }
    lineData.value = line.fromLastOccurrenceOf("=", false, false);
    return lineData;
}

/*
 * Saves data from a desktop entry line to the appropriate DesktopEntry fields.
 */
void DesktopEntry::saveLineData(const LineData& lineData)
{
    using namespace juce;
    using namespace std;
    static const map<Identifier, function<void(const String&, DesktopEntry*)>>
        keyActions =
    {
        {
            "Type", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                if(value == "application")
                {
                    thisEntry->type = Type::application;
                }
                else if(value == "directory")
                {
                    thisEntry->type = Type::directory;
                }
                else if(value == "link")
                {
                    thisEntry->type = Type::link;
                }
                else
                {
                    DBG("DesktopEntry::saveLineData: Invalid entry type "
                            << value << " found in file "
                            << thisEntry->entryFile.getFullPathName());
                    thisEntry->isValid = false;
                }
            }
        },
        {
            "Version", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                if(value != "1.1")
                {
                    DBG("DesktopEntry::saveLineData: Warning, unexpected "
                            << "desktop entry standard version " << value);
                }
            }
        },
        {
            "Name", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                thisEntry->name = value;
            }
        },
        {
            "GenericName", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                thisEntry->genericName = value;
            }
        },
        {
            "NoDisplay", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                thisEntry->noDisplay = (value == "true" || value == "1");
            }
        },
        {
            "Comment", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                thisEntry->comment = value;
            }
        },
        {
            "Icon", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                thisEntry->icon = value;
            }
        },
        {
            "Hidden", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                if(value == "true" || value == "1")
                {
                    DBG("DesktopEntry::saveLineData: Warning: entry file "
                            << thisEntry->entryFile.getFullPathName() 
                            << " should have been deleted, Hidden=true");
                }
            }
        },
        {
            "OnlyShowIn", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                thisEntry->onlyShowIn.addTokens(value, ";,", "\"");
            }
        },
        {
            "NotShowIn", 
            [](const String& value, DesktopEntry* thisEntry)
            {
                thisEntry->notShowIn.addTokens(value, ";,", "\"");
            }
        },
        {
            "DBusActivatable", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "TryExec", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "Path", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "Terminal", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "Actions", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "MimeType", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "Categories", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "Implements", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "Keywords", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "StartupNotify", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "StartupWMClass", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        },
        {
            "URL", 
            [](const String& value, DesktopEntry* thisEntry)
            {
            }
        }
    };
}

/*
 * Saves data from a desktop entry line to the most recently created desktop 
 * action.
 */
void DesktopEntry::saveActionLineData(const LineData& lineData)
{
}

/*
 * Expands all field codes in a command string, removing them and replacing them
 * with appropriate values.
 */
juce::String DesktopEntry::expandFieldCodes
(const juce::String& execString) const
{ 
    using namespace juce;
    juce::Array<int> fieldCodes;
    String expanded = execString;
    for(int i = 0; i < execString.length() - 1; i++)
    {
        if(execString[i] == '%')
        {
           fieldCodes.add(i);
           i++;
        }
    }
    for(int i = fieldCodes.size() - 1; i >= 0; i--)
    {
        String replacementValue;
        switch(execString[i+1])
        {
            // Deprecated field codes to remove.
            case 'd':
            case 'D':
            case 'n':
            case 'N':
            case 'v':
            case 'm':
            // Field codes passing in files or urls, to be ignored.
            case 'f':
            case 'F':
            case 'u':
                break;
            case 'i':
                if(icon.isNotEmpty())
                {
                    replacementValue = String("\"--icon ") + icon + "\"";
                }   
                break;
            case 'c':
                replacementValue = name;
                break;
            case 'k':
                replacementValue = entryFile.getFileName();
                break;
            case '%':
                replacementValue = "%";
                break;
            default:
                DBG("DesktopEntry::" << __func__ << ": Invalid field code %"
                        << execString[i+1]
                        << " found, can't get launch command.");
                return String();
        }
        expanded = expanded.substring(0,i) + replacementValue
                + expanded.substring(i+2);    
    }
    return expanded;
}

/*
 * Checks if a line from a desktop entry file contains a section header title.
 */
bool DesktopEntry::isHeaderLine(const juce::String& line)
{
    return line.startsWithChar('[') && line.endsWithChar(']');
}

/*
 * Extracts a section header title from a desktop entry file line.
 */
juce::String DesktopEntry::extractHeader(const juce::String& headerLine)
{
    return headerLine.substring(1, headerLine.length() - 1);
}

/*
 * Checks if a section header name describes the main desktop entry data 
 * section.
 */
bool DesktopEntry::isMainDataHeader(const juce::String& header)
{
}

/*
 * Checks if a section header name describes a desktop entry action.
 */
bool DesktopEntry::isValidActionHeader(const juce::String& header)
{
}
   
/*
 * Checks if a string is non-empty and contains only valid characters allowed in
 * desktop entry files.
 */
bool DesktopEntry::isValidString(const juce::String& string)
{
}

/*
 * Parses a list of strings from a single string value read from a desktop entry
 * file.
 */
juce::StringArray DesktopEntry::parseList(const juce::String& listStr)
{
    using namespace juce;
    StringArray list;
    // Split on semicolon or comma.
    // Semicolon is the standard, but comma-separated lists were once valid and
    // should still be supported.
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
static bool parseBool(const juce::String& boolStr)
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
        // Error: invalid bool! TODO: do something about it.
        return false;
    }
}
