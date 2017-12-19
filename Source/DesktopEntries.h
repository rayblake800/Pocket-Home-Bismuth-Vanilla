/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DesktopEntries.h
 * Author: anthony
 *
 * Created on December 14, 2017, 1:36 PM
 */

#ifndef DESKTOPENTRIES_H
#define DESKTOPENTRIES_H


#include <vector>
#include <map>
#include <string>
#include "DesktopEntry.h"

class DesktopEntries {
public:
    DesktopEntries();
    DesktopEntries(const DesktopEntries& orig);
    virtual ~DesktopEntries();
    void printAll();
    int size();
    DesktopEntry getEntry(int index);
    std::vector<DesktopEntry*> getCategoryEntries(std::string category);
    std::vector<std::string> getCategoryNames();
private:
    std::vector<DesktopEntry> entries;
    std::vector<std::string> categoryNames;
    std::map<std::string,std::vector<DesktopEntry*>> categories;
 
};

#endif /* DESKTOPENTRIES_H */

