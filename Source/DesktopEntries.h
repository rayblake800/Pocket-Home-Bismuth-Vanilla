/* 
 * File:   DesktopEntries.h
 * Author: anthony
 *
 * Created on December 14, 2017, 1:36 PM
 * Finds all .Desktop and .Directory files in the system,
 * and stores and sorts them as DesktopEntry objects.
 */

#ifndef DESKTOPENTRIES_H
#define DESKTOPENTRIES_H


#include <map>
#include <set>
#include "DesktopEntry.h"
#include "Utils.h"
#include "../JuceLibraryCode/JuceHeader.h"
class DesktopEntries {
public:
    DesktopEntries();
    DesktopEntries(const DesktopEntries& orig);
    virtual ~DesktopEntries();
    
    /**
     * @return the number of stored DesktopEntry objects
     */
    int size();
    
    /**
     * Get all DesktopEntry objects with a given category name
     * @param category the category name
     * @return a set of all matching DesktopEntries
     */
    std::set<DesktopEntry> getCategoryEntries(String category);
    
    /**
     * Get a list of all DesktopEntry objects within several categories
     * @param categoryList one or more category values
     * @return all DesktopEntry objects with at least one of the category values
     * in categories
     */
    std::set<DesktopEntry> getCategoryListEntries(std::vector<String> categoryList);
    
    /**
     * Get the list of all categories found in all desktop entries.
     * @return a set of all category entries
     */
    std::set<String> getCategories();
    
private:
    //list of all entries
    std::set<DesktopEntry> entries;
    //maps category names to lists of entries
    std::map<String,std::set<DesktopEntry>> categories;
 
};

#endif /* DESKTOPENTRIES_H */

