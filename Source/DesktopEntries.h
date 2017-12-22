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
#include "../JuceLibraryCode/JuceHeader.h"
class DesktopEntries {
public:
    DesktopEntries();
    DesktopEntries(const DesktopEntries& orig);
    virtual ~DesktopEntries();
    void printAll();
    int size();
    DesktopEntry getEntry(int index);
    std::vector<DesktopEntry> getCategoryEntries(String category);
    
    /**
     * Get the list of all categories found in all
     * desktop entries
     * @return the category names
     */
    std::vector<String> getCategoryNames();
    
    /**
     * Get the list of main categories specified by 
     * the desktop menu specification
     * @param excludeUnused if true, don't return categories unless
     * at least one application is using them.
     * @return the category names
     */
    std::vector<String> getMainCategories(bool excludeUnused);
private:
    std::vector<DesktopEntry> entries;
    std::vector<String> categoryNames;
    std::map<String,std::vector<DesktopEntry>> categories;
 
};

#endif /* DESKTOPENTRIES_H */

