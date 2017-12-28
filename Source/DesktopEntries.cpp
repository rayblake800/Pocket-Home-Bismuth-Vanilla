/* 
 * File:   DesktopEntries.cpp
 * Author: anthony
 * 
 * Created on December 14, 2017, 1:36 PM
 */
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
        getHomePath() + "/.local/share/applications",
        "/usr/share/applications",
        "/usr/local/share/applications"
    };
    //track entry names and ignore duplicates
    std::set<String> files{};
    for (int i = 0; i < dirs.size(); i++) {
        std::vector<String> dfiles = listFiles(dirs[i]);
        for (int i2 = 0; i2 < dfiles.size(); i2++) {
            files.insert(dirs[i] + "/" + dfiles[i2]);
        }
    }
    DBG(String("Reading ") + String(files.size()) + " potential desktop files");
    //read in files as DesktopEntry objects
    std::regex dfileMatch(".*\\.(desktop|directory)$", std::regex::icase);
    for (std::set<String>::iterator it = files.begin();
            it != files.end(); it++) {
        String path = *it;
        if (std::regex_search(path.toStdString(), dfileMatch)) {
            DesktopEntry de(path, localeName);
            if (de.hidden() || de.noDisplay())continue;
            std::vector<String> deCategories = de.getCategories();
            if(deCategories.empty()){
                deCategories.push_back("Other");
            }
            deCategories.push_back("All");
            for (String category : deCategories) {
                this->categories[category].insert(de);
            }
            entries.insert(de);
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


//Get a list of all DesktopEntry objects within several categories

std::set<DesktopEntry> DesktopEntries::getCategoryListEntries(std::vector<String> categoryList) {
    std::set<DesktopEntry> categoryEntries;
    for (String& category : categoryList) {
        std::set<DesktopEntry> catEntries = getCategoryEntries(category);
        for (DesktopEntry entry : catEntries) {
            categoryEntries.insert(entry);
        }
    }
    return categoryEntries;
}

//Get all DesktopEntries with a given category name

std::set<DesktopEntry> DesktopEntries::getCategoryEntries(String category) {
    return categories[category];
}

//Get the list of all categories found in all desktop entries.

std::set<String> DesktopEntries::getCategories() {
    std::set<String> categoryNames;
    for(std::map<String,std::set<DesktopEntry>>::iterator it = categories.begin();
            it!= categories.end();it++){
        categoryNames.insert(it->first);
    }
    return categoryNames;
}
