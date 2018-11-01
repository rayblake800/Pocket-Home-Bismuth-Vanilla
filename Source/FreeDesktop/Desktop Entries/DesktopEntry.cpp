#include <set>
#include "Locale.h"
#include "Utils.h"
#include "XDGDirectories.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryUtils.h"
#include "DesktopEntry.h"

/* Directory where desktop entries are stored within $XDG_DATA_DIRS */
static const juce::String entryDirectory = "/applications/";

/**
 * @brief Creates a DataConverter that gets and sets typical string values.
 *
 * @param stringParam     The name of a string parameter in DesktopEntry.
 *
 * @param isLocaleString  Whether that string holds localized data supporting
 *                        non-ASCII printable characters.
 *
 * @return                The DataConverter that gets and sets the string
 *                        parameter.
 */
#define STRING_CONVERTER(stringParam, isLocaleString)                          \
{                                                                              \
    .readValue = [](DesktopEntry* thisEntry, const juce::String& value)        \
    {                                                                          \
        thisEntry->stringParam = DesktopEntryUtils::processStringValue         \
            (value, thisEntry->entryFile, isLocaleString);                     \
    },                                                                         \
    .getValue = [](DesktopEntry* thisEntry)->juce::String                      \
    {                                                                          \
        return DesktopEntryUtils::addEscapeSequences(thisEntry->stringParam);  \
    }                                                                          \
}

/**
 * @brief Creates a DataConverter that gets and sets typical list values.
 *
 * @param listParam       The name of a list parameter in DesktopEntry.
 *
 * @param isLocaleString  Whether that list holds localized data supporting
 *                        non-ASCII printable characters.
 *
 * @return                The DataConverter that gets and sets the list
 *                        parameter.
 */
#define LIST_CONVERTER(listParam, isLocaleString)                              \
{                                                                              \
    .readValue = [](DesktopEntry* thisEntry, const juce::String& value)        \
    {                                                                          \
        thisEntry->listParam = DesktopEntryUtils::parseList                    \
            (value, thisEntry->entryFile, isLocaleString);                     \
    },                                                                         \
    .getValue = [](DesktopEntry* thisEntry)->juce::String                      \
    {                                                                          \
        return DesktopEntryUtils::listString(thisEntry->listParam,             \
                isLocaleString);                                               \
    }                                                                          \
}

/**
 * @brief Creates a DataConverter that gets and sets typical boolean values.
 *
 * @param stringParam     The name of a bool parameter in DesktopEntry.
 *
 * @return                The DataConverter that gets and sets the bool
 *                        parameter.
 */
#define BOOL_CONVERTER(boolParam)                                              \
{                                                                              \
    .readValue = [](DesktopEntry* thisEntry, const juce::String& value)        \
    {                                                                          \
        thisEntry->boolParam = DesktopEntryUtils::parseBool                    \
            (value, thisEntry->entryFile);                                     \
    },                                                                         \
    .getValue = [](DesktopEntry* thisEntry)->juce::String                      \
    {                                                                          \
        return DesktopEntryUtils::boolString(thisEntry->boolParam);            \
    }                                                                          \
}

/* Stores all data keys defined in the desktop entry specifications,
   mapped to functions for importing and exporting that key's data.  */
const std::map<juce::Identifier, DesktopEntry::DataConverter> 
DesktopEntry::keyGuide
{
    { "Name",             STRING_CONVERTER(name, true)            },
    { "GenericName",      STRING_CONVERTER(genericName, true)     },
    { "Comment",          STRING_CONVERTER(comment, true)         },
    { "Icon",             STRING_CONVERTER(icon, true)            },
    { "TryExec",          STRING_CONVERTER(tryExec, false)        },
    { "Path",             STRING_CONVERTER(path, false)           },   
    { "StartupWMClass",   STRING_CONVERTER(startupWMClass, false) },
    { "URL",              STRING_CONVERTER(url, false)            },
    { "OnlyShowIn",       LIST_CONVERTER(onlyShowIn, false)       },
    { "NotShowIn",        LIST_CONVERTER(notShowIn, false)        },
    { "Actions",          LIST_CONVERTER(actionTypes, false)      },
    { "MimeType",         LIST_CONVERTER(mimeTypes, false)        },
    { "Categories",       LIST_CONVERTER(categories, false)       },
    { "Implements",       LIST_CONVERTER(implements, false)       },
    { "Keywords",         LIST_CONVERTER(keywords, true)          },
    { "NoDisplay",        BOOL_CONVERTER(noDisplay)               },
    { "DBusActivatable",  BOOL_CONVERTER(dBusActivatable)         },
    { "Terminal",         BOOL_CONVERTER(terminal)                },
    { "StartupNotify",    BOOL_CONVERTER(startupNotify)           }, 
    { "Type",
        {
            .readValue = []
            (DesktopEntry* thisEntry, const juce::String& value)
            {
                using namespace juce;
                const std::map <juce::String, Type> typeMap =
                {
                    { "Application", Type::application },
                    { "Link",        Type::link }
                };
                auto searchIter = typeMap.find(value);
                if(searchIter == typeMap.end())
                {
                    const String errorMessage = String("Invalid entry type ") 
                        + value + " encountered";
                    throw DesktopEntryFileError(thisEntry->entryFile,
                            errorMessage);
                }
                return searchIter->second;
            },
            .getValue = []
            (DesktopEntry* thisEntry)->juce::String
            {
                switch(thisEntry->type)
                {
                    case Type::application:
                        return "Application";
                    case Type::link:
                        return "Link";
                }
                DBG("DesktopEntry: Unhandled entry type!");
                return juce::String();
            }
        }
    },
    { "Version",
       {
            .readValue = []
            (DesktopEntry* thisEntry, const juce::String& value)
            {
                if(value != "1.1")
                {
                    //DBG("DesktopEntry::saveLineData: Warning, unexpected "
                    //        << "desktop entry standard version " << value);
                }
            },
            .getValue = []
            (DesktopEntry* thisEntry)->juce::String
            {
                return "1.1";
            }
        }
    },
    { "Hidden",
        {
            .readValue = []
            (DesktopEntry* thisEntry, const juce::String& value)
            {
                if(DesktopEntryUtils::parseBool(value, thisEntry->entryFile))
                {
                    const juce::String errorMessage
                        ("File marked hidden, should have been deleted");
                    throw DesktopEntryFileError(thisEntry->entryFile,
                            errorMessage);
                }
            },
            .getValue = []
            (DesktopEntry* thisEntry)->juce::String
            {
                return DesktopEntryUtils::boolString(false);
            }
        }
    },
    {
        "Exec",
        {
            .readValue = []
            (DesktopEntry* thisEntry, const juce::String& value)
            {
                using namespace DesktopEntryUtils;
                juce::String execString = processStringValue(value, 
                        thisEntry->entryFile, false);
                thisEntry->exec = unquoteCommandFields(execString);
            },
            .getValue = []
            (DesktopEntry* thisEntry)->juce::String
            {
                using namespace DesktopEntryUtils;
                juce::String execString = addEscapeSequences(thisEntry->exec);
                return quoteCommandFields(execString);
            }
        }
    }
};

/*
 * Loads desktop entry data from a .desktop or .directory file.
 */
DesktopEntry::DesktopEntry
(const juce::File& entryFile, const juce::String& desktopFileID) :
entryFile(entryFile), desktopFileID(desktopFileID)
{
    readEntryFile();
}

/*
 *  Creates a desktop entry object without an existing file.
 */
DesktopEntry::DesktopEntry
(const juce::String& name, const juce::String& desktopFileID, const Type type) :
desktopFileID(desktopFileID)
{
    using namespace juce;
    String filename = desktopFileID.replaceCharacter
        ('-', File::getSeparatorChar());
    // Validate filename:
    bool atElementBeginning = true;
    for(int i = 0; i < filename.length(); i++)
    {
        char testChar = filename[i];
        if((atElementBeginning && testChar == '.')
                || (testChar >= '0' && testChar <= '9'))
        {
            DBG("DesktopEntry::DesktopEntry: Invalid element starting char "
                << testChar << " in file name " << filename);
            throw DesktopEntryFormatError(filename);
        }
        if(! ((testChar >= 'A' && testChar <= 'Z')
                    || (testChar >= 'a' && testChar <= 'z')
                    || (testChar >= '0' && testChar <= '9')
                    || (testChar == '-')
                    || (testChar == '_')
                    || (testChar == '.')))
        {
            DBG("DesktopEntry::DesktopEntry: Invalid character "
                << testChar << " in file name " << filename);
            throw DesktopEntryFormatError(filename);
        }
        atElementBeginning = (testChar == '.');
    }
    if(atElementBeginning)
    {
        DBG("DesktopEntry::DesktopEntry: Filename should not end in a period! "
                << " Filename=" << filename);
        throw DesktopEntryFormatError(filename);
    }
    entryFile = File(XDGDirectories::getUserDataPath() + entryDirectory
            + filename + ".desktop"); 
    // Check if the file exists already, and if so, read data from it.
    if(entryFile.existsAsFile())
    {
        readEntryFile();
    }
    // Apply name and type, possibly replacing existing file values.
    this->type = type;
    setName(name);
}

/*
 * Checks if two desktop entries have the same desktop file ID.
 */
bool DesktopEntry::operator==(const DesktopEntry& toCompare) const
{
    return desktopFileID == toCompare.desktopFileID;
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
    String command = exec;
    if(terminal && command.isNotEmpty())
    {
        Config::MainFile config;
        command = config.getConfigValue<String>
                (Config::MainKeys::termLaunchCommandKey) + " " + command;
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
    using namespace DesktopEntryUtils;
    String outFileText = "";
    String locale = Locale::getLocaleName();
    Array<Identifier> foundKeys;

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
        if(isHeaderLine(line))
        {
            sectionHeader = extractHeader(line);
        }
        // Parse lines that aren't section headers, empty, or comments if under 
        // the main data header.
        else if(!line.startsWithChar('#')
                && line.isNotEmpty()
                && isMainDataHeader(sectionHeader))
        {
            String lineLocale = parseLocale(line);
            if (lineLocale.isEmpty()  || lineLocale == locale)
            {
                try
                {
                    const juce::Identifier& key = parseKey(line);
                    foundKeys.add(key);
                    outFileText += key.toString();
                    if (lineLocale.isNotEmpty())
                    {
                        outFileText += "[";
                        outFileText += lineLocale;
                        outFileText += "]";
                    }
                    outFileText += "=";
                    outFileText += getValue(key);
                    continue;
                }
                catch(DesktopEntryFormatError e) { } 
                // Non-standard key, just copy the line without further
                // processing.
            }
        }
        // Copy field headers, unexpected lines, comments, Actions, 
        // nonstandard keys, and data from other locales unedited.
        outFileText += line;
    }
    // Add the main group header if no source file exists.
    if(outFileText.isEmpty())
    {
        outFileText = DesktopEntryUtils::getMainHeader() + "\n";
    }
    // Copy keys not found in the original file
    for(auto keyIter = keyGuide.begin(); keyIter != keyGuide.end(); keyIter++)
    {
        if (!foundKeys.contains(keyIter->first.toString()))
        {
            String value = keyIter->second.getValue(this);
            if(value.isNotEmpty())
            {
                outFileText += String("\n") + keyIter->first + String("=")
                    + value;
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
    auto searchIter = keyGuide.find(key);
    if(searchIter == keyGuide.end())
    {
        throw DesktopEntryFormatError(key.toString());
    }
    return searchIter->second.getValue(this);
}

/*
 * Loads all desktop entry data from the desktop entry's file.
 */
void DesktopEntry::readEntryFile()
{
    using namespace juce;
    using namespace DesktopEntryUtils;
    StringArray lines;
    if(!entryFile.existsAsFile())
    {
        String errorMessage("File does not exist.");
        throw DesktopEntryFileError(entryFile, errorMessage);
    }
    entryFile.readLines(lines);
    String locale = Locale::getLocaleName();
    /* Last group header read: */
    String groupHeader;
    /* Action ID, if the current header defines an action: */
    String actionId;
    /*If true, the current group is a custom one that should be skipped: */
    bool skipCurrentGroup = false;
    for (const String& line : lines)
    {
        if(line.isEmpty() || line.startsWithChar('#'))
        {
            continue; //skip comments and empty lines
        }
        else if(isHeaderLine(line))
        {
            skipCurrentGroup = false;
            groupHeader = extractHeader(line);
            if(isValidActionHeader(groupHeader))
            {
                actions.add(Action());
            }
            else if(!isMainDataHeader(groupHeader))
            {
                DBG("DesktopEntry::" << __func__ 
                        << ": Ignoring nonstandard group "
                        << groupHeader);
                skipCurrentGroup = true;
            }
            continue;
        }
        if(skipCurrentGroup)
        {
            continue;
        }
        String lineLocale = parseLocale(line);
        if(lineLocale.isEmpty() || lineLocale == locale)
        {
            try
            {
                const juce::Identifier& key = parseKey(line);
                String value = parseValue(line);
                if(isMainDataHeader(groupHeader))
                {
                    saveLineData(key, value);
                }
                else if(groupHeader.isNotEmpty())
                {
                    saveActionLineData(key, value);
                }
            }
            catch(DesktopEntryFormatError e)
            {
                if(e.getBadValue() != line)
                {
                    //DBG("DesktopEntry::" << __func__ 
                    //        << ": Skipping unexpected key " 
                    //        << e.getBadValue());
                    continue;
                }
                else
                {
                    throw e;
                }
            }
        }
    }
}

/*
 * Saves data from a desktop entry line to the appropriate DesktopEntry fields.
 */
void DesktopEntry::saveLineData
(const juce::Identifier& key, const juce::String& value)
{
    using namespace juce;
    auto searchIter = keyGuide.find(key);
    if(searchIter == keyGuide.end())
    {
        throw DesktopEntryFormatError(key.toString());
    }
    searchIter->second.readValue(this, value);
}

/*
 * Saves data from a desktop entry line to the most recently created desktop 
 * action.
 */
void DesktopEntry::saveActionLineData
(const juce::Identifier& key, const juce::String& value)
{
    //TODO: Don't assume action names and action groups are in the same order.
    using namespace juce;
    if(actions.size() == 0)
    {
        //no action found yet!
        jassertfalse;
        return;
    }
    if(key == StringRef("Name"))
    {
        actions.getReference(actions.size() - 1).name = value;
    }
    else if(key == StringRef("Icon"))
    {
        actions.getReference(actions.size() - 1).icon = value;
    }
    else if(key == StringRef("Exec"))
    {
        actions.getReference(actions.size() - 1).exec = value;
    }
    else
    {
        DBG("DesktopEntry::" << __func__ << ": Skipping unexpected action data "
                << key.toString() << " = " << value);
    }
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
