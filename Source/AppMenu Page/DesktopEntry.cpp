/* 
 * File:   DesktopEntry.cpp
 * Author: anthony
 * 
 * Created on December 14, 2017, 1:46 PM
 */

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <regex>
#include "../Utils.h"
#include "DesktopEntries.h"
#include "DesktopEntry.h"

std::map<String, String> DesktopEntry::iconPaths;
bool DesktopEntry::iconPathsMapped;

//initialize string and bool maps with all valid keys

void DesktopEntry::mapInit() {
    std::vector<String> stringKeys = {
        "Version",
        "Name",
        "GenericName",
        "Comment",
        "Icon",
        "OnlyShowIn",
        "NotShowIn",
        "TryExec",
        "Exec",
        "Path",
        "Actions",
        "MimeType",
        "Categories",
        "Implements",
        "Keywords",
        "StartupWMClass",
        "URL"
    };
    std::vector<String> boolKeys{
        "NoDisplay",
        "Hidden",
        "DBusActivatable",
        "Terminal",
        "StartupNotify"};
    //populate maps and write default values:
    for (int i = 0; i < stringKeys.size(); i++) {
        appStrings[String(stringKeys[i])] = "";
    }
    for (int i = 0; i < boolKeys.size(); i++) {
        appBools[String(boolKeys[i])] = false;
    }
}

//Load DesktopEntry data from a .desktop or .directory file

DesktopEntry::DesktopEntry(String path, String localeName) :
entrypath(path) {
    mapInit();
    try {
        std::ifstream desktopFile(path.toStdString());
        //capture "key[locale]=val" lines from the .Desktop/.Directory file
        std::regex varCapture("([A-Za-z]+)(\\[[a-zA-Z0-9_@]+\\])?\\=(.*)");
        std::smatch varMatch;
        for (std::string line; getline(desktopFile, line);) {
            if (line.substr(0, 1) == "#")continue; //skip comments
            if (std::regex_search(line, varMatch, varCapture)) {
                String key;
                String locale;
                String val;
                key = varMatch.str(1);
                if (varMatch.size() > 3) {
                    locale = varMatch.str(2);
                    val = varMatch.str(3);
                } else {
                    val = varMatch.str(2);
                }
                if (locale.isEmpty() || locale == localeName) {
                    std::map<String, String>::iterator strSearch =
                            appStrings.find(key);
                    if (key == "Type") {
                        if (val == "Application") {
                            type = APPLICATION;
                        } else if (val == "Link") {
                            type = LINK;
                        } else if (val == "Directory") {
                            type = DIRECTORY;
                        } else {
                            //DBG("invalid type:" + val);
                        }
                    } else if (strSearch != appStrings.end()) {
                        appStrings[key] = val;
                    } else {
                        std::map < String, bool>::iterator boolSearch =
                                appBools.find(key);
                        if (boolSearch != appBools.end()) {
                            if (val == "true") {
                                appBools[key] = true;
                            } else if (val == "false") {
                                appBools[key] = false;
                            } else {
                                //DBG("invalid boolean pair " + key + ":" + val);
                            }
                        } //else DBG("unrecognized data:" + key + "=" + val);
                    }
                }
            } //else DBG("no match:" + line);
        }
        desktopFile.close();
    } catch (std::ifstream::failure e) {
        DBG("couldn't open " + path);
    }
}

//Creates a DesktopEntry object representing an application category

DesktopEntry::DesktopEntry(MainConfigFile::AppFolder appFolder) {
    mapInit();
    type = DIRECTORY;
    appStrings["Name"] = appFolder.name;
    appStrings["Icon"] = appFolder.icon;
    for (int i = 0; i < appFolder.categories.size(); i++) {
        if (i > 0)appStrings["Categories"] += ";";
        appStrings["Categories"] += appFolder.categories[i];
    }
}

//Create a DesktopEntry object with data from the config file

DesktopEntry::DesktopEntry(MainConfigFile::AppItem appItem) {
    mapInit();
    type = APPLICATION;
    appStrings["Name"] = appItem.name;
    appStrings["Icon"] = appItem.icon;
    appStrings["Exec"] = appItem.shell;
}

DesktopEntry::DesktopEntry(const DesktopEntry& orig) {
    entrypath = orig.entrypath;
    appStrings = orig.appStrings;
    appBools = orig.appBools;
    type = orig.type;
}

DesktopEntry::~DesktopEntry() {
}

bool DesktopEntry::operator==(const DesktopEntry toCompare) const {
    return getName() == toCompare.getName();
}

bool DesktopEntry::operator<(const DesktopEntry toCompare) const {
    return getName() < toCompare.getName();
}

String DesktopEntry::getName() const {
    try {
        return appStrings.at("Name");
    } catch (std::out_of_range e) {
        return "";
    }
}

DesktopEntry::Type DesktopEntry::getType() {
    return type;
}

String DesktopEntry::getVersion() {
    return appStrings["Version"];
}

String DesktopEntry::getGenericName() {
    return appStrings["GenericName"];
}

String DesktopEntry::getComment() {
    return appStrings["Comment"];
}

String DesktopEntry::getIconPath() {
    String icon = appStrings["Icon"];
    //DBG(String("Searching for ")+icon);
    //if the icon variable is a full path, return that
    if (icon.substring(0, 1) == "/")return icon;
    //otherwise check the iconPaths map
    //if icon is a partial path, trim it
    if (icon.contains("/"))icon = icon.substring(1 + icon.lastIndexOf("/"));
    //if icon contains file extensions, trim them
    std::regex iconPattern("^(.+)\\.(png|svg|xpm)$", std::regex::ECMAScript | std::regex::icase);
    std::smatch iconMatch;
    std::string searchString = icon.toStdString();
    if (std::regex_search(searchString, iconMatch, iconPattern))icon = iconMatch.str(1);
    if (!iconPathsMapped)mapIcons();
    String fullPath = iconPaths[icon];
    if (fullPath.isEmpty()) {
        //DBG(String("DesktopEntry::Couldn't find icon ") + icon);
        for (std::map<String, String>::iterator it = iconPaths.begin();
                it != iconPaths.end(); it++) {
            String iconCandidate = it->first;
            if (!it->second.isEmpty() &&
                    (iconCandidate.containsIgnoreCase(icon) ||
                    icon.containsIgnoreCase(iconCandidate))) {
                //DBG(icon+String("~")+iconCandidate+String("=")+it->second);
                iconPaths[icon] = it->second;
                return it->second;
            }
        }
        return type == DIRECTORY ? String(DEFAULT_DIRECTORY_ICON_PATH) :
                String(DEFAULT_APP_ICON_PATH);
    }
    return fullPath;
}

String DesktopEntry::getExec() {
    return appStrings["Exec"];
}

String DesktopEntry::getPath() {
    return appStrings["Path"];
}

String DesktopEntry::getStartupWMClass() {
    return appStrings["StartupWm"];
}

String DesktopEntry::getURL() {
    return appStrings["Exec"];
}

std::vector<String> DesktopEntry::getActions() {
    return split(appStrings["Actions"], ";");
}

std::vector<String> DesktopEntry::getMimeType() {
    return split(appStrings["MimeType"], ";");
}

std::vector<String> DesktopEntry::getCategories() {
    String categories = appStrings["Categories"];
    if (categories.isEmpty())categories = "Other";
    return split(categories, ";");
}

std::vector<String> DesktopEntry::getImplements() {
    return split(appStrings["Implements"], ";");
}

std::vector<String> DesktopEntry::getKeywords() {
    return split(appStrings["Keywords"], ";");
}

bool DesktopEntry::onlyShowIn(String env) {
    String showIn = appStrings["OnlyShowIn"];
    return showIn.contains(env);
}

bool DesktopEntry::notShowIn(String env) {
    String showIn = appStrings["NotShowIn"];
    return showIn.contains(env);
}

bool DesktopEntry::tryExec() {
    return fileExists(appStrings["TryExec"]);
}

bool DesktopEntry::noDisplay() {
    return appBools["NoDisplay"];
}

bool DesktopEntry::hidden() {
    return appBools["Hidden"];
}

bool DesktopEntry::dBusActivatable() {
    return appBools["DBusActivatable"];
}

bool DesktopEntry::terminal() {
    return appBools["Terminal"];
}

bool DesktopEntry::startupNotify() {
    return appBools["StartupNotify"];
}

void DesktopEntry::mapIcons() {
    std::set<String> ignore = {
        "actions",
        "applets",
        "cursors",
        "devices",
        "emblems",
        "emotes",
        "mimetypes",
        "places",
        "status",
        "stock",
        "symbolic"
    };
    Array<String> searchPaths;
    //check user and pocket-home directories before all else
    searchPaths.add("/usr/share/pocket-home/appIcons/");
    searchPaths.add(getHomePath() + "/.icons/");
    searchPaths.add(getHomePath() + "/.local/share/icons");
    //Recursively traverse all subdirectories at a given path, and add all valid
    //ones to the list of paths to search
    std::function<void(String) > findSearchPaths;
    findSearchPaths = [&findSearchPaths, &ignore, &searchPaths, this]
            (String path) {
        std::vector<String> dirs = listDirectoryFiles(path);
        if (dirs.empty())return;
        //sort icon size directories, if found
        std::regex sizePattern("^([0-9]+)");
        std::smatch sizeMatch;
        std::sort(dirs.begin(), dirs.end(), [&sizeMatch, &sizePattern](const String& a, const String & b)->bool {
            std::string a_str = a.toStdString();
            std::string b_str = b.toStdString();
            if (a == b)return false;
                    int aVal = 0;
                    int bVal = 0;
                if (std::regex_search(a_str, sizeMatch, sizePattern)) {
                    sscanf(sizeMatch.str(1).c_str(), "%d", &aVal);
                }
            if (std::regex_search(b_str, sizeMatch, sizePattern)) {
                sscanf(sizeMatch.str(1).c_str(), "%d", &bVal);
            }
            if (aVal != bVal) {
                //higher numbers first, until after 128px
                if (aVal > 128 && aVal < 999)aVal = 1 + 128 / aVal;
                    if (bVal > 128 && bVal < 999)bVal = 1 + 128 / bVal;
                        return aVal > bVal;
                    }
            return false;
        });
        //DBG(String("Searching ")+String(dirs.size())+String(" subdirectories"));
        for (const String& subDir : dirs) {
            //only add directories outside of the ignore set
            if (ignore.find(subDir) == ignore.end()) {
                String subPath = path + subDir + "/";
                if (!searchPaths.contains(subPath)) {
                    searchPaths.add(subPath);
                    findSearchPaths(subPath);
                }
            }
        };
        iconPathsMapped = true;

    };

    //build a list of primary search directories
    std::vector<String> checkPaths;
    std::vector<String> basePaths = {
        "/usr/share/icons/",
        "/usr/local/icons/",
        "/usr/share/pixmaps/"
    };
    //search for icon theme directories, prioritize them if found
    String iconTheme;
    String fallbackIconTheme;
    std::string configPath = getHomePath().toStdString() + "/.gtkrc-2.0";
    if (fileExists(configPath)) {
        std::regex themeMatch("gtk-icon-theme-name=\"(.+)\"");
        std::regex fallbackThemeMatch("gtk-fallback=icon-theme=\"(.+)\"");
        try {
            std::ifstream file(configPath);
            std::smatch match;
            for (std::string line; getline(file, line);) {
                if (std::regex_search(line, match, themeMatch)) {
                    themeMatch = (match.str(1));
                } else if (std::regex_search(line, match, fallbackThemeMatch)) {
                    fallbackIconTheme = (match.str(1));
                }
                if (iconTheme.isNotEmpty() && fallbackIconTheme.isNotEmpty())break;
            }
            file.close();
        } catch (std::ifstream::failure f) {
            DBG("DesktopEntry:failed to read icon theme from .gtkrc-2.0");
        }
    }
    if (iconTheme.isNotEmpty()) {
        for (int i = 0; i < basePaths.size(); i++) {
            checkPaths.push_back(basePaths[i] + iconTheme + "/");
        }
    }
    if (!fallbackIconTheme.isEmpty()) {
        for (int i = 0; i < basePaths.size(); i++) {
            checkPaths.push_back(basePaths[i] + fallbackIconTheme + "/");
        }
    }
    for (int i = 0; i < basePaths.size(); i++) {
        checkPaths.push_back(basePaths[i]);
    }

    //run recursive mapping for all subdirectories
    for (int i = 0; i < checkPaths.size(); i++) {
        //DBG(String("Mapping icon files under ") + checkPaths[i]);
        findSearchPaths(checkPaths[i]);
    }
    DBG(String("Searching ")+String(searchPaths.size())+" icon paths:");
    //finally, find and map icon files
    for (const String& path : searchPaths) {
        std::vector<String> files = listFiles(path);
        std::regex iconPattern("^(.+)\\.(png|svg|xpm)$", std::regex::ECMAScript | std::regex::icase);
        std::smatch iconMatch;
        for (const String& file : files) {
            std::string fileStr = file.toStdString();
            if (std::regex_search(fileStr, iconMatch, iconPattern)) {
                String filename = iconMatch.str(1);
                if (this->iconPaths[filename].isEmpty()) {
                    this->iconPaths[filename] = path + iconMatch.str(0);
                    //DBG(filename+String("=")+path+iconMatch.str(0));
                }
            }
        }
    }
    DBG(String("Mapped ")+String(iconPaths.size())+String(" icon files."));
}