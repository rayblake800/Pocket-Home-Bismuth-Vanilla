/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DesktopEntry.cpp
 * Author: anthony
 * 
 * Created on December 14, 2017, 1:46 PM
 */

#include "DesktopEntry.h"
#include "DesktopEntries.h"
#include "sysutils.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include <stdio.h>

DesktopEntry::DesktopEntry(std::string path, std::string localeName) {
    entrypath = path;
    std::vector<std::string> stringKeys = {
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
    std::vector<std::string> boolKeys{
        "NoDisplay",
        "Hidden",
        "DBusActivatable",
        "Terminal",
        "StartupNotify"};
    //populate maps and write default values:
    for (int i = 0; i < stringKeys.size(); i++) {
        appStrings[std::string(stringKeys[i])] = "";
    }
    for (int i = 0; i < boolKeys.size(); i++) {
        appBools[std::string(boolKeys[i])] = false;
    }
    try {
        std::ifstream file(path);
        //capture groups: key,locale,value
        std::regex re("([A-Za-z]+)(\\[[a-zA-Z0-9_@]+\\])?\\=(.*)");
        std::smatch match;
        for (std::string line; getline(file, line);) {
            if (line.substr(0, 1) == "#")continue; //skip comments
            if (std::regex_search(line, match, re)) {
                std::string key;
                std::string locale;
                std::string val;
                key = match.str(1);
                if (match.size() > 3) {
                    locale = match.str(2);
                    val = match.str(3);
                } else {
                    val = match.str(2);
                }
                if (locale.empty() || locale == localeName) {
                    std::map<std::string, std::string>::iterator ssearch =
                            appStrings.find(key);
                    if (key == "Type") {
                        if (val == "Application") {
                            type = APPLICATION;
                        } else if (val == "Link") {
                            type = LINK;
                        } else if (val == "Directory") {
                            type = DIRECTORY;
                        } else {
                            std::cout << "invalid type:" << val << "\n";
                        }
                    } else if (ssearch != appStrings.end()) {
                        appStrings[key] = val;
                    } else {
                        std::map < std::string, bool>::iterator bsearch =
                                appBools.find(key);
                        if (bsearch != appBools.end()) {
                            if (val == "true") {
                                appBools[key] = true;
                            } else if (val == "false") {
                                appBools[key] = false;
                            } else {
                                std::cout << "invalid boolean pair " << key <<
                                        ":" << val << "\n";
                            }
                        } else {
                            //std::cout << "unrecognized data:" << key <<
                            //        "=" << val << "\n";
                        }

                    }
                }
            } else {
                //std::cout << "no match:" << line << "\n";
            }
        }
        file.close();
    } catch (std::ifstream::failure e) {
        std::cout << "couldn't open " << path << "\n";
    }
}

DesktopEntry::DesktopEntry(const DesktopEntry& orig) {
    entrypath = orig.entrypath;
    appStrings = orig.appStrings;
    appBools = orig.appBools;
    type = orig.type;
    iconPath = orig.iconPath;
}

DesktopEntry::~DesktopEntry() {
}

std::string DesktopEntry::getName() {
    return appStrings["Name"];
}

std::string DesktopEntry::getVersion() {
    return appStrings["Version"];
}

std::string DesktopEntry::getGenericName() {
    return appStrings["GenericName"];
}

std::string DesktopEntry::getComment() {
    return appStrings["Comment"];
}

std::string DesktopEntry::getIconPath() {
    if (!iconPath.empty())return iconPath;
    return findIconPath();
}

std::string DesktopEntry::getExec() {
    return appStrings["Exec"];
}

std::string DesktopEntry::getPath() {
    return appStrings["Path"];
}

std::string DesktopEntry::getStartupWMClass() {
    return appStrings["StartupWm"];
}

std::string DesktopEntry::getURL() {
    return appStrings["Exec"];
}

std::vector<std::string> DesktopEntry::getActions() {
    return split(appStrings["Actions"], ';');
}

std::vector<std::string> DesktopEntry::getMimeType() {
    return split(appStrings["MimeType"], ';');
}

std::vector<std::string> DesktopEntry::getCategories() {
    std::string categories=appStrings["Categories"];
    if(categories.empty())categories="Other";
    return split(categories, ';');
}

std::vector<std::string> DesktopEntry::getImplements() {
    return split(appStrings["Implements"], ';');
}

std::vector<std::string> DesktopEntry::getKeywords() {
    return split(appStrings["Keywords"], ';');
}

bool DesktopEntry::onlyShowIn(std::string env) {
    std::string showIn = appStrings["OnlyShowIn"];
    int i = showIn.find(env);
    return i != std::string::npos;
}

bool DesktopEntry::notShowIn(std::string env) {
    std::string showIn = appStrings["NotShowIn"];
    int i = showIn.find(env);
    return i != std::string::npos;
}

bool DesktopEntry::tryExec() {
    return fileExists(appStrings["TryExec"]);
}

bool DesktopEntry::noDisplay(){
    return appBools["NoDisplay"];
}
bool DesktopEntry::hidden(){
    return appBools["Hidden"];
}
bool DesktopEntry::dBusActivatable(){
    return appBools["DBusActivatable"];
}
bool DesktopEntry::terminal(){
    return appBools["Terminal"];
}
bool DesktopEntry::startupNotify(){
    return appBools["StartupNotify"];
}

inline bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string DesktopEntry::searchIconPaths(std::string icon, std::string path) {
    using namespace std;
    vector<string> files = listFiles(path);
    string iconFile;
    regex iconMatch(icon + "(\\.(png|svg|xpm))?$", regex::ECMAScript | regex::icase);

    foreach(files, [path, &iconFile, iconMatch](string file)->bool {
        if (regex_search(file, iconMatch)) {
            iconFile = path + file;
            return true;
        }
        return false;
    });
    if (!iconFile.empty())return iconFile;
    //check subdirectories if not already found
    vector<string> dirs = listDirectoryFiles(path);
    //cout<<depth<<":path="<<path<<"\n";
    //prioritize higher-res icon directories
    if (dirs.empty())return iconFile;
    regex re("^([0-9]+)");
    smatch numMatch;
    sort(dirs.begin(), dirs.end(), [&numMatch, &re](const string& a, const string & b)->bool {
        if (a == b)return false;
        int aVal = 0;
        int bVal = 0;
        if (regex_search(a, numMatch, re)) {
            sscanf(numMatch.str(1).c_str(), "%d", &aVal);
        }
        if (regex_search(b, numMatch, re)) {
            sscanf(numMatch.str(1).c_str(), "%d", &bVal);
        }
        if (aVal != bVal) {
            //higher numbers first, until after 128px
            if (aVal > 128)aVal = 1 + 128 / aVal;
                if (bVal > 128)bVal = 1 + 128 / bVal;
                    return aVal > bVal;
                }
        return false;
    });
    //cout<<"sorted:\n";
    //foreach(dirs,[](string s)->bool{cout<<"\t"<<s<<"\n";return false;});

    foreach(dirs,[icon, path,this, &iconFile](string s)->bool {
        iconFile = searchIconPaths(icon, path + s + "/");
        return !iconFile.empty();
    });
    return iconFile;

}

std::string DesktopEntry::findIconPath() {
    std::vector<std::string> checkPaths;
    std::string icon = appStrings["Icon"];
    if (icon.empty())return "";

    //explicit path defined, return it
    if (icon.substr(0, 1) == "/") {
        iconPath = icon;
        return "abs:" + iconPath;
    }

    std::vector<std::string> basePaths = {
        getHomePath() + "/.icons/",
        "/usr/local/icons/",
        "/usr/share/icons/",
        "/usr/share/pixmaps/"
    };

    //if no explicit path, find all directories to search
    //check under system theme first
    std::string theme = getTheme();
    if (!theme.empty()) {
        for (int i = 0; i < basePaths.size(); i++) {
            checkPaths.push_back(basePaths[i] + theme + "/");
        }
    }
    //if it didn't turn up there, try the fallback theme
    std::string fallbackTheme = getFallbackTheme();
    if (!fallbackTheme.empty()) {
        for (int i = 0; i < basePaths.size(); i++) {
            checkPaths.push_back(basePaths[i] + fallbackTheme + "/");
        }
    }
    //search all icon files if a theme-specific one doesn't show up
    for (int i = 0; i < basePaths.size(); i++) {
        checkPaths.push_back(basePaths[i]);
    }

    //actually search directories until the icon file turns up
    for (int i = 0; i < checkPaths.size(); i++) {
        iconPath = searchIconPaths(icon, checkPaths[i]);
        if (!iconPath.empty())break;
    }
    if (iconPath.empty())return "MISSING:" + icon;
    return iconPath;
}
