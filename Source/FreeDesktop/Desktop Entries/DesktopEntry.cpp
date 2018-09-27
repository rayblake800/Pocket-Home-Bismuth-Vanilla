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
DesktopEntry::DesktopEntry(juce::File entryFile) :
entryFile(entryFile)
{
    using namespace juce;
    StringArray lines;
    entryFile.readLines(lines);
    String locale = Localized::getLocaleName();
    String groupHeader;
    for (const String& line : lines)
    {
        if(line.startsWithChar('#'))
        {
            continue; //skip comments
        }
        else if(line.startsWithChar('[') && line.endsWithChar(']'))
        {
            groupHeader = line.substring(1, line.length() - 1);
            if(groupHeader.startsWith(actionPrefix))
            {
                String actionName = groupHeader.fromFirstOccurrenceOf
                    (actionPrefix, false, false);
                if(actionName.isNotEmpty())
                {
                    Action newAction;
                    newAction.name = actionName;
                    actions.add(newAction);
                }
                else
                {
                    DBG("DesktopEntry::DesktopEntry: Invalid group header "
                            << groupHeader);
                }
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
 * Creates an empty desktop entry object. 
 */
DesktopEntry::DesktopEntry(juce::String name) : 
name(name),
entryFile(XDGDirectories::getUserDataPath() 
        + entryDirectory + name + ".desktop") 
{ }

/*
 * Gets the desktop entry's type.
 */
DesktopEntry::Type DesktopEntry::getType() const
{
    return type;
}

/*
 * Sets the desktop entry's type.
 */
void DesktopEntry::setType(const Type newType)
{
    type = newType;
}

/*
 * Gets the desktop entry's name.
 */
juce::String DesktopEntry::getName() const
{
    return name;
}

/*
 * Sets the desktop entry's name
 */
void DesktopEntry::setName(const juce::String& newName)
{
    name = newName;
}

/*
 * Gets a generic name describing the entry.
 */
juce::String DesktopEntry::getGenericName() const
{
    return genericName;
}

/*
 * Sets the generic name describing the entry.
 */
void DesktopEntry::setGenericName(const juce::String& newGenericName)
{
    genericName = newGenericName;
}

/*
 * Checks if this desktop entry should appear in application menus.
 */
bool DesktopEntry::shouldBeDisplayed() const
{
    return !noDisplay && onlyShowIn.isEmpty();
}

/*
 * Sets if this desktop entry should appear in application menus.
 */
void DesktopEntry::setIfDisplayed(const bool showEntry)
{
    noDisplay = !showEntry;
}

/*
 * Gets the name or path of the desktop entry's icon.
 */
juce::String DesktopEntry::getIconName() const
{
    return icon;
}

/*
 * Sets the name or path of the desktop entry's icon.
 */
void DesktopEntry::setIconName(const juce::String& newIcon)
{
    icon = newIcon;
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
juce::String DesktopEntry::getExecString() const
{
    return exec;
}

/*
 * Sets the string value used to contstruct the launch command.
 */
void DesktopEntry::setExecString(const juce::String& newExec)
{
    exec = newExec;
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
 * Sets the application name used when checking if this application is valid.
 */
void DesktopEntry::setTryExec(const juce::String& newTryExec)
{
    tryExec = newTryExec;
}

/*
 * Gets the path where this application should run.
 */
juce::String DesktopEntry::getRunDirectory() const
{
    return path;
}

/*
 * Sets the path where this application should run.
 */
void DesktopEntry::setRunDirectory(const juce::String& runningDirectory)
{
    path = runningDirectory;
}

/*
 * Checks if this application should be launched in a new terminal window.
 */
bool DesktopEntry::getLaunchedInTerm() const
{
    return terminal;
}

/*
 * Sets if this application should be launched in a new terminal window.
 */
void DesktopEntry::setLaunchedInTerm(const bool termLaunch)
{
    terminal = termLaunch;
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
 * Sets the list of categories associated with this desktop entry.
 */
void DesktopEntry::setCategories(const juce::StringArray& newCategories)
{
    categories = newCategories;
}

/*
 * Gets a list of keywords associated with this desktop entry.
 */
juce::StringArray DesktopEntry::getKeywords() const
{
    return keywords;
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
bool DesktopEntry::writeFile()
{
    using namespace juce;
    if(!isDataValid())
    {
        return false;
    }
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
    return outFile.replaceWithText(outFileText);
}
          
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

/*
 * Checks if this desktop entry's data follows the desktop entry standard.
 */
bool DesktopEntry::isDataValid() const
{
    if(type == Type::application && exec.isEmpty())
    {
        return false;
    }
    // Check for proper character escaping:
    // Check for all required fields:
    return true;  //replace this!
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
            return lineData;
        }
        lineData.locale = line.substring(openIndex+1, closeIndex);
    }
    lineData.value = line.fromLastOccurrenceOf("=", false, false);
    return lineData;
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

static juce::StringArray parseList(const juce::String listStr)
{
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
                if(value == "directory")
                {
                    thisEntry->type = Type::directory;
                }
                else if(value == "link")
                {
                    thisEntry->type = Type::link;
                }
                else
                {
                    thisEntry->type = Type::application;
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
 * Given a standard desktop entry data key, get the value mapped to that key.
 */
juce::String DesktopEntry::getValue(const juce::Identifier& key)
{
}

/*
 * Gets the desktop file ID of the desktop entry.
 *
 */
juce::String DesktopEntry::getDesktopFileId() const
{
}


