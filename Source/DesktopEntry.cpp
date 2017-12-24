/* 
 * File:   DesktopEntry.cpp
 * Author: anthony
 * 
 * Created on December 14, 2017, 1:46 PM
 */

#include "DesktopEntry.h"
#include "DesktopEntries.h"
#include "Utils.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <regex>

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
                            DBG("invalid type:" + val);
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
                                DBG("invalid boolean pair " + key + ":" + val);
                            }
                        } else DBG("unrecognized data:" + key + "=" + val);
                    }
                }
            } else DBG("no match:" + line);
        }
        desktopFile.close();
    } catch (std::ifstream::failure e) {
        DBG("couldn't open " + path);
    }
}

//Creates a DesktopEntry object representing an application category

DesktopEntry::DesktopEntry(String category) {
    mapInit();
    type = DIRECTORY;
    appStrings["Name"] = category;
    appStrings["Icon"] = category;
    appStrings["Exec"] = "OPEN:" + category;
}

//Create a DesktopEntry object with data from the config file

DesktopEntry::DesktopEntry(const var &entryJson) {
    mapInit();
    type = APPLICATION;
    appStrings["Name"] = entryJson["name"];
    appStrings["Icon"] = entryJson["icon"];
    appStrings["Exec"] = entryJson["shell"];
}

DesktopEntry::DesktopEntry(const DesktopEntry& orig) {
    entrypath = orig.entrypath;
    appStrings = orig.appStrings;
    appBools = orig.appBools;
    type = orig.type;
}

DesktopEntry::~DesktopEntry() {
}

String DesktopEntry::getName() {
    return appStrings["Name"];
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
    //if the icon variable is a full path, return that
    if (icon.substring(0, 1) == "/")return icon;
    //otherwise check the iconPaths map
    //if icon is a partial path, trim it
    if(icon.contains("/"))icon=icon.substring(1+icon.lastIndexOf("/"));
    //if icon contains file extensions, trim them
    std::regex iconPattern("^(.+)\\.(png|svg)$", std::regex::ECMAScript | std::regex::icase);
    std::smatch iconMatch;
    std::string searchString=icon.toStdString();
    if(std::regex_search(searchString,iconMatch,iconPattern))icon=iconMatch.str(1);
    if (!iconPathsMapped)mapIcons();
    String fullPath = iconPaths[icon];
    if (fullPath.isEmpty()) {
        DBG(String("DesktopEntry::Couldn't find icon ") + icon);
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
    std::function<void(String) > recursiveIconSearch;
    recursiveIconSearch = [&recursiveIconSearch, this](String path) {
        //first, map image files with new names
        std::vector<String> files = listFiles(path);
        std::regex iconPattern("^(.+)\\.(png|svg)$", std::regex::ECMAScript | std::regex::icase);
        std::smatch iconMatch;
        foreach(files, [path, &iconPattern, &iconMatch, this](String file)->bool {
            std::string fileStr = file.toStdString();
            if (std::regex_search(fileStr, iconMatch, iconPattern)) {
                String filename = iconMatch.str(1);
                if (this->iconPaths[filename].isEmpty()) {
                    this->iconPaths[filename] = path + iconMatch.str(0);
                }
            }
            return false;
        });
        //then recursively search subdirectories
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
                if (aVal > 128)aVal = 1 + 128 / aVal;
                    if (bVal > 128)bVal = 1 + 128 / bVal;
                        return aVal > bVal;
                    }
            return false;
        });
        foreach(dirs, [path, this, &recursiveIconSearch](String subDir)->bool {
            recursiveIconSearch(path + subDir + "/");
        });
        iconPathsMapped = true;

    };

    //build a list of primary search directories
    std::vector<String> checkPaths;
    std::vector<String> basePaths = {
        getHomePath() + "/.icons/",
        "/usr/share/pocket-home/appIcons/",
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
            DBG("DesktopEntry:failed to read icon them from .gtkrc-2.0");
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

    //finally, run recursive mapping for all directories
    for (int i = 0; i < checkPaths.size(); i++) {
        DBG(String("Mapping icon files under ") + checkPaths[i]);
        recursiveIconSearch(checkPaths[i]);
    }
}