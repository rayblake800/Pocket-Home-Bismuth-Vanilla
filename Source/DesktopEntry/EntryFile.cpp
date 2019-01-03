#define DESKTOP_ENTRY_IMPLEMENTATION
#include "DesktopEntry/EntryFile.h"
#include "Locale.h"
#include "Utils.h"
#include "XDGDirectories.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "DesktopEntry/Exceptions/FileError.h"
#include "DesktopEntry/Exceptions/FormatError.h"
#include "DesktopEntry/FileUtils.h"

/* Directory where desktop entries are stored within $XDG_DATA_DIRS */
static const juce::String entryDirectory = "/applications/";

/**
 * @brief Creates a DataConverter that gets and sets typical string values.
 *
 * @param stringParam     The name of a desktop entry string parameter.
 *
 * @param isLocaleString  Whether that string holds localized data supporting
 *                        non-ASCII printable characters.
 *
 * @return                The DataConverter that gets and sets the string
 *                        parameter.
 */
#define STRING_CONVERTER(stringParam, isLocaleString)                          \
{                                                                              \
    .readValue = [](DesktopEntry::EntryFile* thisEntry,                        \
        const juce::String& value)                                             \
    {                                                                          \
        thisEntry->stringParam = DesktopEntry::FileUtils::processStringValue   \
            (value, thisEntry->file, isLocaleString);                          \
    },                                                                         \
    .getValue = [](DesktopEntry::EntryFile* thisEntry)->juce::String           \
    {                                                                          \
        return DesktopEntry::FileUtils::addEscapeSequences                     \
            (thisEntry->stringParam);                                          \
    }                                                                          \
}

/**
 * @brief Creates a DataConverter that gets and sets typical list values.
 *
 * @param listParam       The name of a desktop entry list parameter.
 *
 * @param isLocaleString  Whether that list holds localized data supporting
 *                        non-ASCII printable characters.
 *
 * @return                The DataConverter that gets and sets the list
 *                        parameter.
 */
#define LIST_CONVERTER(listParam, isLocaleString)                              \
{                                                                              \
    .readValue = [](DesktopEntry::EntryFile* thisEntry,                        \
        const juce::String& value)                                             \
    {                                                                          \
        thisEntry->listParam = DesktopEntry::FileUtils::parseList              \
            (value, thisEntry->file, isLocaleString);                          \
    },                                                                         \
    .getValue = [](DesktopEntry::EntryFile* thisEntry)->juce::String           \
    {                                                                          \
        return DesktopEntry::FileUtils::listString(thisEntry->listParam,       \
            isLocaleString);                                                   \
    }                                                                          \
}

/**
 * @brief Creates a DataConverter that gets and sets typical boolean values.
 *
 * @param stringParam     The name of a desktop entry boolean parameter.
 *
 * @return                The DataConverter that gets and sets the bool
 *                        parameter.
 */
#define BOOL_CONVERTER(boolParam)                                              \
{                                                                              \
    .readValue = [](DesktopEntry::EntryFile* thisEntry,                        \
        const juce::String& value)                                             \
    {                                                                          \
        thisEntry->boolParam = DesktopEntry::FileUtils::parseBool              \
            (value, thisEntry->file);                                          \
    },                                                                         \
    .getValue = [](DesktopEntry::EntryFile* thisEntry)->juce::String           \
    {                                                                          \
        return DesktopEntry::FileUtils::boolString(thisEntry->boolParam);      \
    }                                                                          \
}

/* Stores all data keys defined in the desktop entry specifications,
   mapped to functions for importing and exporting that key's data.  */
const std::map<juce::Identifier, DesktopEntry::EntryFile::DataConverter> 
DesktopEntry::EntryFile::keyGuide
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
            (DesktopEntry::EntryFile* thisEntry, const juce::String& value)
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
                    throw FileError(thisEntry->file,
                        errorMessage);
                }
                return searchIter->second;
            },
            .getValue = []
            (DesktopEntry::EntryFile* thisEntry)->juce::String
            {
                switch(thisEntry->type)
                {
                    case Type::application:
                        return "Application";
                    case Type::link:
                        return "Link";
                }
                DBG("DesktopEntry::EntryFile: Unhandled entry type!");
                return juce::String();
            }
        }
    },
    { "Version",
       {
            .readValue = []
            (DesktopEntry::EntryFile* thisEntry, const juce::String& value)
            {
                if(value != "1.1")
                {
                    //DBG("DesktopEntry::EntryFile::saveLineData: "
                    // << "Warning, unexpected desktop entry standard version " 
                    // << value);
                }
            },
            .getValue = []
            (DesktopEntry::EntryFile* thisEntry)->juce::String
            {
                return "1.1";
            }
        }
    },
    { "Hidden",
        {
            .readValue = []
            (DesktopEntry::EntryFile* thisEntry, const juce::String& value)
            {
                if(DesktopEntry::FileUtils::parseBool(value, thisEntry->file))
                {
                    const juce::String errorMessage
                        ("File marked hidden, should have been deleted");
                    throw FileError(thisEntry->file, errorMessage);
                }
            },
            .getValue = []
            (DesktopEntry::EntryFile* thisEntry)->juce::String
            {
                return DesktopEntry::FileUtils::boolString(false);
            }
        }
    },
    { "Exec",
        {
            .readValue = []
            (DesktopEntry::EntryFile* thisEntry, const juce::String& value)
            {
                juce::String execString 
                    = DesktopEntry::FileUtils::processStringValue(value, 
                    thisEntry->file, false);
                thisEntry->exec = DesktopEntry::FileUtils::unquoteCommandFields
                    (execString);
            },
            .getValue = []
            (DesktopEntry::EntryFile* thisEntry)->juce::String
            {
                juce::String execString 
                    = DesktopEntry::FileUtils::addEscapeSequences
                    (thisEntry->exec);
                return DesktopEntry::FileUtils::quoteCommandFields(execString);
            }
        }
    }
};

/*
 * Loads desktop entry data from a .desktop file.
 */
DesktopEntry::EntryFile::EntryFile
(const juce::File& sourceFile, const juce::String& desktopFileID) :
file(sourceFile), desktopFileID(desktopFileID)
{
    readEntryFile();
}

/*
 *  Creates a desktop entry object without an existing file.
 */
DesktopEntry::EntryFile::EntryFile
(const juce::String& name, const juce::String& desktopFileID, const Type type) :
desktopFileID(desktopFileID)
{
    using juce::String;
    using juce::File;
    String filename = desktopFileID;
    // Validate filename:
    bool atElementBeginning = true;
    for(int i = 0; i < filename.length(); i++)
    {
        char testChar = filename[i];
        if(atElementBeginning && (testChar == '.'
                || (testChar >= '0' && testChar <= '9')))
        {
            DBG("DesktopEntry::EntryFile::EntryFile: Invalid element starting"
                " char " << testChar << " in file name " << filename);
            throw FormatError(filename);
        }
        if(! ((testChar >= 'A' && testChar <= 'Z')
                    || (testChar >= 'a' && testChar <= 'z')
                    || (testChar >= '0' && testChar <= '9')
                    || (testChar == '-')
                    || (testChar == '_')
                    || (testChar == '.')))
        {
            DBG("DesktopEntry::EntryFile::EntryFile: Invalid character "
                << testChar << " in file name " << filename);
            throw FormatError(filename);
        }
        atElementBeginning = (testChar == '.');
    }
    if(atElementBeginning)
    {
        DBG("DesktopEntry::EntryFile::EntryFile: Filename should not end in a"
            << " period! " << " Filename=" << filename);
        throw FormatError(filename);
    }
    file = File(XDGDirectories::getUserDataPath() + entryDirectory
        + filename + ".desktop"); 
    // Check if the file exists already, and if so, read data from it.
    if(file.existsAsFile())
    {
        readEntryFile();
    }
    // Apply name and type, possibly replacing existing file values.
    this->type = type;
    setName(name);
}


/*
 * Checks if the EntryFile does not contain all data required by a valid desktop
 * entry.
 */
bool DesktopEntry::EntryFile::isMissingData() const
{
    return name.isEmpty() || (exec.isEmpty() && !dBusActivatable);
}

/*
 * Checks if two desktop entries have the same desktop file ID.
 */
bool DesktopEntry::EntryFile::operator==(const EntryFile& toCompare) const
{
    return desktopFileID == toCompare.desktopFileID;
}
    
/*
 * Alphabetically compares entries based on their names.
 */
bool DesktopEntry::EntryFile::operator<(const EntryFile& toCompare) const
{
    return name.toUpperCase() < toCompare.name.toUpperCase();
}

/*
 * Gets the desktop entry's type.
 */
DesktopEntry::EntryFile::Type DesktopEntry::EntryFile::getType() const
{
    return type;
}

/*
 * Gets the unique desktop file ID that identifies this entry.
 */
juce::String DesktopEntry::EntryFile::getDesktopFileID() const
{
    return desktopFileID;
}
    
/*
 * Gets the desktop entry's name.
 */
juce::String DesktopEntry::EntryFile::getName() const
{
    return name;
}

/*
 * Gets a generic name describing the entry.
 */
juce::String DesktopEntry::EntryFile::getGenericName() const
{
    return genericName;
}

/*
 * Checks if this desktop entry should appear in application menus.
 */
bool DesktopEntry::EntryFile::shouldBeDisplayed() const
{
    return !noDisplay && onlyShowIn.isEmpty();
}

/*
 * Gets the name or path of the desktop entry's icon.
 */
juce::String DesktopEntry::EntryFile::getIcon() const
{
    return icon;
}

/*
 * Gets the command string used to launch this entry's application.
 */
juce::String DesktopEntry::EntryFile::getLaunchCommand() const
{
    using namespace juce;
    String command = exec;
    if(terminal && command.isNotEmpty())
    {
        Config::MainFile config;
        command = config.getTermLaunchPrefix() + " " + command;
    }
    return command;
}

/*
 * Gets the string value used to construct the launch command.
 */
juce::String DesktopEntry::EntryFile::getExec() const
{
    return exec;
}

/*
 * Gets an application name or path to check when determining if this 
 * entry's application is valid.
 */
juce::String DesktopEntry::EntryFile::getTryExec() const
{
    return tryExec;
}

/*
 * Gets the path where this application should run.
 */
juce::String DesktopEntry::EntryFile::getRunDirectory() const
{
    return path;
}

/*
 * Checks if this entry's application should be launched in a new terminal 
 * window.
 */
bool DesktopEntry::EntryFile::getLaunchedInTerm() const
{
    return terminal;
}

/*
 * Gets the names of all alternate actions supported by this desktop entry.
 */
juce::StringArray DesktopEntry::EntryFile::getActionIDs() const
{
    return actionTypes;
}

/*
 * Gets the display title of a desktop entry action.
 */
juce::String DesktopEntry::EntryFile::getActionTitle
(const juce::String actionID) const
{
    if(actions.count(actionID) == 0)
    {
        return juce::String();
    }
    return actions.at(actionID).title;
}

/*
 * Gets the name or path of an action's icon.
 */
juce::String DesktopEntry::EntryFile::getActionIcon
(const juce::String actionID) const
{
    if(actions.count(actionID) == 0)
    {
        return juce::String();
    }
    return actions.at(actionID).icon;
}

/*
 * Gets the command used to run a desktop entry action.
 */
juce::String DesktopEntry::EntryFile::getActionLaunchCommand
(const juce::String actionID) const
{
    if(actions.count(actionID) == 0)
    {
        return juce::String();
    }
    return expandFieldCodes(actions.at(actionID).exec);
}

/*
 * Gets the list of categories associated with this desktop entry.
 */
juce::StringArray DesktopEntry::EntryFile::getCategories() const
{
    return categories;
}

/*
 * Gets a list of keywords associated with this desktop entry.
 */
juce::StringArray DesktopEntry::EntryFile::getKeywords() const
{
    return keywords;
}

/*
 * Sets the desktop entry's name.
 */
void DesktopEntry::EntryFile::setName(const juce::String& newName)
{
    name = newName;
}

/*
 * Sets the generic name describing the entry.
 */
void DesktopEntry::EntryFile::setGenericName(const juce::String& newGenericName)
{
    genericName = newGenericName;
}

/*
 * Sets if this desktop entry should appear in application menus.
 */
void DesktopEntry::EntryFile::setIfDisplayed(const bool showEntry)
{
    noDisplay = !showEntry;
}

/*
 * Sets the name or path of the desktop entry's icon.
 */
void DesktopEntry::EntryFile::setIcon(const juce::String& newIcon)
{
    icon = newIcon;
}

/*
 * Sets the string value used to construct the entry's launch command.
 */
void DesktopEntry::EntryFile::setExec(const juce::String& newExec)
{
    exec = newExec;
}

/*
 * Sets the application name used when checking if this entry's application is 
 * valid.
 */
void DesktopEntry::EntryFile::setTryExec(const juce::String& newTryExec)
{
    tryExec = newTryExec;
}

/*
 * Sets the path where this entry's application should run.
 */
void DesktopEntry::EntryFile::setRunDirectory
(const juce::String& runningDirectory)
{
    path = runningDirectory;
}

/*
 * Sets if this entry's application should be launched in a new terminal window.
 */
void DesktopEntry::EntryFile::setLaunchedInTerm(const bool termLaunch)
{
    terminal = termLaunch;
}

/*
 * Sets the list of categories associated with this desktop entry.
 */
void DesktopEntry::EntryFile::setCategories
(const juce::StringArray& newCategories)
{
    categories = newCategories;
}

/*
 * Sets the list of keywords associated with this desktop entry.
 */
void DesktopEntry::EntryFile::setKeywords(const juce::StringArray& newKeywords)
{
    keywords = newKeywords;
}

/*
 * Writes this desktop entry to the user's local application data directory.
 */
void DesktopEntry::EntryFile::writeFile()
{
    using juce::String;
    String outFileText = "";
    String locale = Locale::getLocaleName();
    juce::Array<juce::Identifier> foundKeys;
    juce::Array<juce::Identifier> foundLocaleKeys;

    // Reload the source file to preserve comments and alternate locale data.
    juce::StringArray lines;
    file.readLines(lines);
    String sectionHeader;
    for(const String& line : lines)
    {
        if(outFileText.isNotEmpty() && line.isNotEmpty())
        {
            outFileText += "\n";
        }
        // Copy comments and empty lines without further processing
        if(line.startsWithChar('#') || line.isEmpty())
        {
            outFileText += line;
            continue;
        }
        // Find and copy section headers
        else if(FileUtils::isHeaderLine(line))
        {
            sectionHeader = FileUtils::extractHeader(line);
        }
        // Parse lines that aren't section headers, empty, or comments if under 
        // the main data header.
        else if(line.isNotEmpty() && FileUtils::isMainDataHeader(sectionHeader))
        {
            String lineLocale = FileUtils::parseLocale(line);
            if (lineLocale.isEmpty()  || lineLocale == locale)
            {
                try
                {
                    const juce::Identifier& key = FileUtils::parseKey(line);
                    if(foundLocaleKeys.contains(key) || (lineLocale.isEmpty()
                            && foundKeys.contains(key)))
                    {
                       DBG("DesktopEntry::EntryFile::" << __func__
                               << ": Skipping duplicate key " 
                               << key.toString());
                    }
                    else
                    {
                        foundKeys.add(key);
                        outFileText += key.toString();
                        if (lineLocale.isNotEmpty())
                        {
                            foundLocaleKeys.add(key);
                            outFileText += "[";
                            outFileText += lineLocale;
                            outFileText += "]";
                        }
                        outFileText += "=";
                        outFileText += getValue(key);
                    }
                    continue;
                }
                catch(FormatError e) 
                { 
                    // Non-standard key, just copy the line without further
                    // processing.
                    DBG("DesktopEntry::EntryFile::" << __func__
                            << ": Copying non-standard line="
                            << line);
                }
            }
        }
        // If no header has been located, the main header must be added before
        // any non-comment content.
        if(sectionHeader.isEmpty())
        {
            sectionHeader = FileUtils::getMainHeader();
            outFileText += sectionHeader + "\n";
        }
        // Copy field headers, unexpected lines, Actions, nonstandard keys, and 
        // data from other locales unedited.
        outFileText += line;
    }
    // Add the main group header if no header has been found yet.
    if(sectionHeader.isEmpty())
    {
        outFileText += FileUtils::getMainHeader() + "\n";
    }
    // Copy keys not found in the original file
    for(auto keyIter : keyGuide)
    {
        if (!foundLocaleKeys.contains(keyIter.first)
                && !foundKeys.contains(keyIter.first))
        {
            String value = keyIter.second.getValue(this);
            if(value.isNotEmpty())
            {
                outFileText += String("\n") + keyIter.first + String("=")
                    + value;
            }
        }

    }
    // Write files to the user data directory:
    juce::File outFile(XDGDirectories::getUserDataPath()
        + entryDirectory + file.getFullPathName().fromLastOccurrenceOf
        (entryDirectory, false, false));
    outFile.create();
    DBG("DesktopEntry::EntryFile::" << __func__ << ": writing file " 
        << outFile.getFullPathName()); 
    outFile.replaceWithText(outFileText);
}

/*
 * Given a standard desktop entry data key, get the value mapped to that key.
 */
juce::String DesktopEntry::EntryFile::getValue(const juce::Identifier& key)
{
    auto searchIter = keyGuide.find(key);
    if(searchIter == keyGuide.end())
    {
        throw FormatError(key.toString());
    }
    return searchIter->second.getValue(this);
}

/*
 * Loads all desktop entry data from the desktop entry's file.
 */
void DesktopEntry::EntryFile::readEntryFile()
{
    using juce::StringArray;
    using juce::String;
    StringArray lines;
    if(!file.existsAsFile())
    {
        String errorMessage("File does not exist.");
        throw FileError(file, errorMessage);
    }
    file.readLines(lines);
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
        else if(FileUtils::isHeaderLine(line))
        {
            skipCurrentGroup = false;
            groupHeader = FileUtils::extractHeader(line);
            if(FileUtils::isValidActionHeader(groupHeader))
            {
                groupHeader = FileUtils::extractActionID(groupHeader);
            }
            else if(!FileUtils::isMainDataHeader(groupHeader))
            {
                DBG("DesktopEntry::EntryFile::" << __func__ 
                        << ": Ignoring nonstandard group " << groupHeader);
                skipCurrentGroup = true;
            }
            continue;
        }
        if(skipCurrentGroup)
        {
            continue;
        }
        String lineLocale = FileUtils::parseLocale(line);
        if(lineLocale.isEmpty() || lineLocale == locale)
        {
            try
            {
                const juce::Identifier& key = FileUtils::parseKey(line);
                String value = FileUtils::parseValue(line);
                if(FileUtils::isMainDataHeader(groupHeader))
                {
                    saveLineData(key, value);
                }
                else if(groupHeader.isNotEmpty())
                {
                    saveActionLineData(groupHeader, key, value);
                }
            }
            catch(FormatError e)
            {
                if(e.getBadValue() != line)
                {
                    //DBG("DesktopEntry::EntryFile::" << __func__ 
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
 * Saves data from a desktop entry line to the appropriate EntryFile fields.
 */
void DesktopEntry::EntryFile::saveLineData
(const juce::Identifier& key, const juce::String& value)
{
    auto searchIter = keyGuide.find(key);
    if(searchIter == keyGuide.end())
    {
        throw FormatError(key.toString());
    }
    searchIter->second.readValue(this, value);
}

/*
 * Saves data from a desktop entry line to the most recently created desktop 
 * action.
 */
void DesktopEntry::EntryFile::saveActionLineData(const juce::String actionID, 
        const juce::Identifier& key, const juce::String& value)
{
    using juce::StringRef;
    if(key == StringRef("Name"))
    {
        actions[actionID].title = value;
    }
    else if(key == StringRef("Icon"))
    {
        actions[actionID].icon = value;
    }
    else if(key == StringRef("Exec"))
    {
        actions[actionID].exec = value;
    }
    else
    {
        DBG("DesktopEntry::EntryFile::" << __func__ 
            << ": Skipping unexpected action data "
            << key.toString() << " = " << value);
    }
}

/*
 * Expands all field codes in a command string, removing them and replacing them
 * with appropriate values.
 */
juce::String DesktopEntry::EntryFile::expandFieldCodes
(const juce::String& execString) const
{ 
    using juce::String;
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
            case 'U':
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
                replacementValue = file.getFileName();
                break;
            case '%':
                replacementValue = "%";
                break;
            default:
                DBG("DesktopEntry::EntryFile::" << __func__ 
                    << ": Invalid field code %" << execString[i+1]
                    << " found, can't get launch command.");
                return String();
        }
        expanded = expanded.substring(0,i) + replacementValue
            + expanded.substring(i+2);    
    }
    return expanded;
}
