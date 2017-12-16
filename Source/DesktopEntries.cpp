/*
  ==============================================================================

    DesktopEntries.cpp
    Created: 15 Dec 2017 6:10:43pm
    Author:  anthony

  ==============================================================================
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
#include <stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include "DesktopEntries.h"
#include "sysutils.h"


DesktopEntries::DesktopEntries() {
    std::string localeName=getLocale();
    //read the contents of all desktop application directories
    std::vector<std::string> dirs={
    "~/.local/share/applications",
    "/usr/share/applications",
    "/usr/local/share/applications"
    };
    std::vector<std::string> files {};
    for(int i=0;i<dirs.size();i++){
        std::vector<std::string> dfiles=listFiles(dirs[i]);
        for(int i2=0;i2<dfiles.size();i2++){
            files.push_back(dirs[i]+"/"+dfiles[i2]);
        }
    }
    //read in files as DesktopEntry objects
    std::regex dfileMatch(".*\\.(desktop|directory)$",std::regex::icase);
    for(std::vector<std::string>::iterator it=files.begin();
            it!= files.end();it++){
        std::string path = *it;
        if(std::regex_search(path,dfileMatch)){
            entries.push_back(DesktopEntry(path,localeName));
        }
    }
    std::cout<<"found "<<entries.size()<<" entries\n";
}

DesktopEntries::DesktopEntries(const DesktopEntries& orig) {
    entries=orig.entries;
}

DesktopEntries::~DesktopEntries() {
}


void DesktopEntries::printAll(){
    for(int i=0;i<entries.size();i++){
        std::cout<<"entry "<<i<<":"<<entries[i].getName()<<",exec="
                <<entries[i].getExec()<<"\n";
    }
}

int DesktopEntries::size(){
    return entries.size();
}


DesktopEntry DesktopEntries::getEntry(int index){
    return entries[index];
}



