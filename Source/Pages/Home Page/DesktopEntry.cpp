#include <set>
#include "Utils.h"
#include "DesktopEntry.h"

const String DesktopEntry::localEntryPath = "/~.local/share/applications/";

/**
 * Load DesktopEntry data from a .desktop or .directory file
 */
DesktopEntry::DesktopEntry(File entryFile) :
entrypath(entryFile.getFullPathName())
{
    ScopedPointer<FileInputStream> in = entryFile.createInputStream();
    StringArray lines;
    while (!in->isExhausted())
    {
        lines.add(in->readNextLine());
    }
    String locale = getLocale();
    for (const String& line : lines)
    {
        if (line.substring(0, 1) == "#")continue; //skip comments
        LineValues lineData = getLineData(line);
        if (lineData.key.isNotEmpty() &&
            (lineData.locale.isEmpty() || lineData.locale == locale))
        {
            dataStrings[lineData.key] = lineData.value;
        }
    }
}

DesktopEntry::~DesktopEntry() { }

/**
 * Creates a new desktop entry from parameter data.
 */
DesktopEntry::DesktopEntry(String title, String icon, String command,
        StringArray categories, bool launchInTerminal)
{
    dataStrings[typeKey] = "Application";
    setValue(StringValue::name, title);
    setValue(StringValue::icon, icon);
    setValue(StringValue::exec, command);
    setValue(ListValue::categories, categories);
    setValue(BoolValue::terminal, launchInTerminal);
    int filesFound = 0;
    File newFile(localEntryPath + title + String(".desktop"));
    while (newFile.exists())
    {
        filesFound++;
        newFile = File(localEntryPath + title
                + String(filesFound) + String(".desktop"));
    }
    newFile.create();
    newFile.appendText("[Desktop Entry]");
    entrypath = newFile.getFullPathName();
    writeFile();
}

/**
 * get the file type for this entry
 */
DesktopEntry::Type DesktopEntry::getType() const
{
    String type = dataStrings.at(typeKey);
    if (type == "Application") return application;
    if (type == "Link") return link;
    if (type == "Directory") return directory;

    DBG("DesktopEntry::" << __func__
            << ": invalid type:" << type);
    return application;
}

/**
 * Entries with the same filename are considered equal.
 */
bool DesktopEntry::operator==(const DesktopEntry toCompare) const
{
    String file1 = entrypath.fromLastOccurrenceOf("/", false, false);
    String file2 = toCompare.entrypath.fromLastOccurrenceOf("/", false, false);
    DBG("DesktopEntry::" << __func__ << file1 << "==" << "?");
    return file1 == file2 && file1.isNotEmpty();
}

/**
 * Used for comparing entries by name
 */
bool DesktopEntry::operator<(const DesktopEntry toCompare) const
{
    return getValue(name) < toCompare.getValue(name);
}

/**
 * Get stored string data.
 */
String DesktopEntry::getValue(StringValue valueType) const
{
    try
    {
        return dataStrings.at(getKey(valueType));
    }
    catch (std::out_of_range e)
    {
        return "";
    }
}

/**
 * Get stored boolean data.
 */
bool DesktopEntry::getValue(BoolValue valueType) const
{
    try
    {
        return dataStrings.at(getKey(valueType)) == "true";
    }
    catch (std::out_of_range e)
    {
        return false;
    }
}

/**
 * Get stored list data.
 */
StringArray DesktopEntry::getValue(ListValue valueType) const
{
    try
    {
        return StringArray::fromTokens(dataStrings.at(getKey(valueType)),
                ";", "");
    }
    catch (std::out_of_range e)
    {
        return StringArray();
    }
}

/**
 * Changes the value of stored string data.
 */
void DesktopEntry::setValue(StringValue valueType, String newValue)
{
    dataStrings[getKey(valueType)] = newValue;
}

/**
 * Changes the value of stored boolean data.
 */
void DesktopEntry::setValue(BoolValue valueType, bool newValue)
{
    dataStrings[getKey(valueType)] = newValue ? "true" : "false";
}

/**
 * Changes the value of stored list data.
 */
void DesktopEntry::setValue(ListValue valueType, StringArray newValue)
{
    String joinedList = "";
    for (const String& listItem : newValue)
    {
        if (joinedList.isNotEmpty())
        {
            joinedList += ";";
        }
        joinedList += listItem;
    }
    dataStrings[getKey(valueType)] = joinedList;
}

/**
 * Exports this entry to a .Desktop file.
 */
void DesktopEntry::writeFile()
{
    String outFileText = "";
    String locale = getLocale();
    StringArray foundKeys;
    //Reload the source file to preserve comments and alternate locale data
    StringArray lines = StringArray::fromLines
            (File(entrypath).loadFileAsString());
    for (const String& line : lines)
    {
        if (outFileText.isNotEmpty())
        {
            outFileText += "\n";
        }
        //copy comments unedited
        if (line.substring(0, 1) == "#")
        {
            outFileText += line;
            continue;
        }
        LineValues lineData = getLineData(line);
        if (lineData.key.isNotEmpty()
            && (lineData.locale.isEmpty() || lineData.locale == locale))
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
            outFileText += dataStrings[lineData.key];
        }//copy unexpected lines and other locales unedited
        else
        {
            outFileText += line;
        }
    }
    //copy keys not found in the original file
    for (std::map<String, String>::iterator it = dataStrings.begin();
         it != dataStrings.end(); it++)
    {
        String key = it->first;
        String value = it->second;
        if (value.isNotEmpty() && !foundKeys.contains(value))
        {
            outFileText += String("\n") + key + String("=") + value;
        }

    }
    //Get new file path
    entrypath = localEntryPath + entrypath.fromLastOccurrenceOf("/", false, false);
    File outFile(entrypath);
    outFile.create();
    outFile.replaceWithText(outFileText);
}

/**
 * Get any StringValue's key.
 */
String DesktopEntry::getKey(StringValue valueType) const
{
    switch (valueType)
    {
        case version: return "Version";
        case name: return "Name";
        case genericName: return "GenericName";
        case comment: return "Comment";
        case icon: return "Icon";
        case tryExec: return "TryExec";
        case exec: return "Exec";
        case path: return "Path";
        case startupWMClass: return "StartupWMClass";
        case url: return "URL";
    }
}

/**
 * Get any BoolValue's key.
 */
String DesktopEntry::getKey(BoolValue valueType) const
{
    switch (valueType)
    {
        case noDisplay: return "NoDisplay";
        case hidden: return "Hidden";
        case dBusActivatable: return "DBusActivatable";
        case terminal: return "Terminal";
        case startupNotify: return "StartupNotify";
    }
}

/**
 * Get any ListValue's key.
 */
String DesktopEntry::getKey(ListValue valueType) const
{
    switch (valueType)
    {
        case onlyShowIn: return "OnlyShowIn";
        case notShowIn: return "NotShowIn";
        case actions: return "Actions";
        case mimeType: return "MimeType";
        case categories: return "Categories";
        case implements: return "Implements";
        case keywords: return "Keywords";
    }
}

/**
 * @return the locale name as it would appear in .desktop files
 */
String DesktopEntry::getLocale()
{
    std::locale l("");
    return String(l.name()).initialSectionNotContaining(".");
}

/**
 * Parses desktop entry file lines
 */
DesktopEntry::LineValues DesktopEntry::getLineData(String line)
{
    LineValues values;
    int valueIndex = line.indexOf("=");
    if (valueIndex < 0)
    {
        return values;
    }
    values.value = line.substring(valueIndex + 1);
    line = line.substring(0, valueIndex);
    if (line.contains("[") && line.contains("]"))
    {
        values.locale = line.fromFirstOccurrenceOf("[", false, false)
                .initialSectionNotContaining("]");
        line = line.initialSectionNotContaining("]");
    }
    values.key = line;
    return values;
}
