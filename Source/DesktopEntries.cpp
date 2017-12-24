/* 
 * File:   DesktopEntries.cpp
 * Author: anthony
 * 
 * Created on December 14, 2017, 1:36 PM
 */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <regex>
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "DesktopEntries.h"
#include "Utils.h"

DesktopEntries::DesktopEntries() {
    String localeName = getLocale();
    //read the contents of all desktop application directories
    DBG("finding desktop entries...");
    std::vector<String> dirs = {
        "~/.local/share/applications",
        "/usr/share/applications",
        "/usr/local/share/applications"
    };
    std::vector<String> files{};
    for (int i = 0; i < dirs.size(); i++) {
        std::vector<String> dfiles = listFiles(dirs[i]);
        for (int i2 = 0; i2 < dfiles.size(); i2++) {
            files.push_back(dirs[i] + "/" + dfiles[i2]);
        }
    }
    DBG(String("Reading ") + String(files.size())+" potential desktop files");
    //read in files as DesktopEntry objects
    std::regex dfileMatch(".*\\.(desktop|directory)$", std::regex::icase);
    categoryEntries.push_back(DesktopEntry("All"));
    categoryEntries.push_back(DesktopEntry("Other"));
    for (std::vector<String>::iterator it = files.begin();
            it != files.end(); it++) {
        String path = *it;
        if (std::regex_search(path.toStdString(), dfileMatch)) {
            DesktopEntry de(path, localeName);
            categories["All"].push_back(de);
            std::vector<String> deCats = de.getCategories();
            if (deCats.empty())categories["Other"].push_back(de);
            foreach(deCats, [de, this](String c)->bool {
                if (this->categories[c].empty()) {
                    this->categoryEntries.push_back(DesktopEntry(c));
                }
                this->categories[c].push_back(de);
                return false;
            });
            entries.push_back(de);
        }
    }
    DBG(String("found ") + String(entries.size()) + " entries");
}

DesktopEntries::DesktopEntries(const DesktopEntries& orig) {
    entries = orig.entries;
}

DesktopEntries::~DesktopEntries() {
}

//return the number of stored DesktopEntry objects

int DesktopEntries::size() {
    return entries.size();
}

//Get all DesktopEntries with a given category name

std::vector<DesktopEntry> DesktopEntries::getCategoryEntries(String category) {
    return categories[category];
}

//Get the list of all categories found in all desktop entries.

std::vector<DesktopEntry> DesktopEntries::getCategories() {
    return categoryEntries;
}

/**
 * Get the list of main categories specified by 
 * the desktop menu specification
 */
std::vector<DesktopEntry> DesktopEntries::getMainCategories(bool excludeUnused) {
    std::vector<String> mainCategories = {
        "All",
        "AudioVideo",
        "Audio",
        "Video",
        "Development",
        "Education",
        "Game",
        "Graphics",
        "Network",
        "Office",
        "Science",
        "Settings",
        "System",
        "Utility",
        "Other"
    };
    std::vector<DesktopEntry> categoryEntries;
    for (int i = 0; i < mainCategories.size(); i++) {
        String category = mainCategories[i];
        if (categories[category].size() > 0 || !excludeUnused) {
            DBG(String("Found category ")+category+String(",size ")+String(categories[category].size()));
            categoryEntries.push_back(DesktopEntry(category));
        }
    }
    return categoryEntries;
}