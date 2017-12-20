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
    std::cout<<"finding desktop entries...\n";
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
    std::cout<<"Reading "<<files.size()<<" potential desktop files\n";
    //read in files as DesktopEntry objects
    std::regex dfileMatch(".*\\.(desktop|directory)$",std::regex::icase);
    categoryNames.push_back("All");
    for(std::vector<String>::iterator it=files.begin();
            it!= files.end();it++){
        String path = *it;
        if(std::regex_search(path.toStdString(),dfileMatch)){
            DesktopEntry de(path,localeName);
            std::cout<<de.getName()<<":"<<de.getIconPath()<<"\n";
            categories["All"].push_back(&de);
            std::vector<String> deCats=de.getCategories();
            foreach(deCats,[&de,this](String c)->bool{
                if(this->categories[c].empty()){
                    this->categoryNames.push_back(c);
                }
                this->categories[c].push_back(&de);
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


void DesktopEntries::printAll(){
    std::map<String,std::vector<String>> catMap;
    for(int i=0;i<entries.size();i++){
        DesktopEntry de = entries[i];
        if(de.hidden() || de.noDisplay())continue;
        std::vector<String> cats=de.getCategories();
        foreach(cats,[&catMap,&de](String c)->bool{
            catMap[c].push_back(de.getName());
            return false;
        });
    
    }
    for(std::map<String,std::vector<String>>::iterator it=catMap.begin();
            it!=catMap.end();it++){
        std::cout<<"Category:"<<it->first<<"\n";
        std::vector<String> apps = it->second;
        foreach(apps,[](String app)->bool{
            std::cout<<"\t"<<app<<"\n";
            return false;
        });
    }
}

int DesktopEntries::size(){
    return entries.size();
}


DesktopEntry DesktopEntries::getEntry(int index){
    return entries[index];
}

std::vector<DesktopEntry*> DesktopEntries::getCategoryEntries(String category){
    return categories[category];
}

std::vector<String> DesktopEntries::getCategoryNames(){
    return categoryNames;
}