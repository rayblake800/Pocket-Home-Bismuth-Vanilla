/* 
 * File:   DesktopEntry.cpp
 * Author: anthony
 * 
 * Created on December 14, 2017, 1:46 PM
 */

#include <fstream>
#include <set>
#include <regex>
#include "../Utils.h"
#include "DesktopEntry.h"

const String DesktopEntry::localEntryPath =
        getHomePath() + "/.local/share/applications/";

/**
 * Load DesktopEntry data from a .desktop or .directory file
 */
DesktopEntry::DesktopEntry(String path) :
entrypath(path)
{
    Array<String> lines = split(File(path).loadFileAsString(), "\n");
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

DesktopEntry::DesktopEntry(const DesktopEntry& orig)
{
    entrypath = orig.entrypath;
    dataStrings = orig.dataStrings;
}

DesktopEntry::~DesktopEntry()
{
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

    DBG(String("DesktopEntry::getType():invalid type:") + type);
    return application;
}

/**
 * Entries with the same filename are considered equal.
 */
bool DesktopEntry::operator==(const DesktopEntry toCompare) const
{
    String file1 = entrypath.fromLastOccurrenceOf("/", false, false);
    String file2 = toCompare.entrypath.fromLastOccurrenceOf("/", false, false);
    DBG(file1 + String("==") + file2 + "?");
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
    } catch (std::out_of_range e)
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
    } catch (std::out_of_range e)
    {
        return false;
    }
}

/**
 * Get stored list data.
 */
Array<String> DesktopEntry::getValue(ListValue valueType) const
{
    try
    {
        return split(dataStrings.at(getKey(valueType)), ";");
    } catch (std::out_of_range e)
    {
        return Array<String>();
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
void DesktopEntry::setValue(ListValue valueType, Array<String> newValue)
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
    String outFile = "";
    String locale = getLocale();
    //Reload the source file to preserve comments and alternate locale data
    Array<String> lines = split(File(entrypath).loadFileAsString(), "\n");
    for (const String& line : lines)
    {
        outFile += "\n";
        //copy comments unedited
        if (line.substring(0, 1) == "#")
        {
            outFile += line;
            continue;
        }
        LineValues lineData = getLineData(line);
        if (lineData.key.isNotEmpty()
                && (lineData.locale.isEmpty() || lineData.locale == locale))
        {
            outFile += lineData.key;
            if (lineData.locale.isNotEmpty())
            {
                outFile += "[";
                outFile += lineData.locale;
                outFile += "]";
            }
            outFile += "=";
            outFile += lineData.value;
        }//copy unexpected lines and other locales unedited
        else
        {
            outFile += line;
        }
    }
    //Get new file path
    entrypath = getHomePath() + String();
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

    /**
     * capture "key[locale]=val" lines from the .Desktop/.Directory file
     * 
     * If two groups are captured:
     * group1 = key, 
     * group2 = value
     * 
     * If three groups are captured:
     * group1 = key,
     * group2 = locale,
     * group3 = value
     */
    std::regex varPattern("([A-Za-z]+)(\\[[a-zA-Z0-9_@]+\\])?\\=(.*)");
    std::smatch varMatch;
    std::string stdLine = line.toStdString();
    if (std::regex_search(stdLine, varMatch, varPattern))
    {
        values.key = String(varMatch[1]);
        if (varMatch.size() > 3)
        {
            values.locale = String(varMatch[2]);
            values.value = String(varMatch[3]);
        } else
        {
            values.value = String(varMatch[2]);
        }
    }
    return values;
}
