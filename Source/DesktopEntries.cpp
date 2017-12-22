/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
#include <map>
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "DesktopEntries.h"
#include "Utils.h"


DesktopEntries::DesktopEntries() {
    String localeName=getLocale();
    //read the contents of all desktop application directories
    //std::cout<<"finding desktop entries...\n";
    std::vector<String> dirs={
    "~/.local/share/applications",
    "/usr/share/applications",
    "/usr/local/share/applications"
    };
    std::vector<String> files {};
    for(int i=0;i<dirs.size();i++){
        std::vector<String> dfiles=listFiles(dirs[i]);
        for(int i2=0;i2<dfiles.size();i2++){
            files.push_back(dirs[i]+"/"+dfiles[i2]);
        }
    }
    //std::cout<<"Reading "<<files.size()<<" potential desktop files\n";
    //read in files as DesktopEntry objects
    std::regex dfileMatch(".*\\.(desktop|directory)$",std::regex::icase);
    categoryNames.push_back("All");
    categoryNames.push_back("Other");
    for(std::vector<String>::iterator it=files.begin();
            it!= files.end();it++){
        String path = *it;
        if(std::regex_search(path.toStdString(),dfileMatch)){
            DesktopEntry de(path,localeName);
            categories["All"].push_back(de);
            std::vector<String> deCats=de.getCategories();
            if(deCats.empty())categories["Other"].push_back(de);
            foreach(deCats,[de,this](String c)->bool{
                if(this->categories[c].empty()){
                    this->categoryNames.push_back(c);
                }
                this->categories[c].push_back(de);
                return false;
            });
            entries.push_back(de);
        }
    }
    std::cout<<"found "<<entries.size()<<" entries\n";
}

DesktopEntries::DesktopEntries(const DesktopEntries& orig) {
    entries=orig.entries;
}

DesktopEntries::~DesktopEntries() {
}


int DesktopEntries::size(){
    return entries.size();
}


DesktopEntry DesktopEntries::getEntry(int index){
    return entries[index];
}

std::vector<DesktopEntry> DesktopEntries::getCategoryEntries(String category){
    return categories[category];
}

std::vector<String> DesktopEntries::getCategoryNames(){
    return categoryNames;
}

   /**
     * Get the list of main categories specified by 
     * the desktop menu specification
     * @return the category names
     */
    std::vector<String> DesktopEntries::getMainCategories(bool excludeUnused){
        std::vector<String> mainCategories={
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
        if(excludeUnused){
            std::vector<String> usedCategories;
            for(int i=0;i< mainCategories.size();i++){
                String category=mainCategories[i];
                if(categories[category].size()>0)usedCategories.push_back(category);
            }
            mainCategories=usedCategories;
        }
        return mainCategories;
    }