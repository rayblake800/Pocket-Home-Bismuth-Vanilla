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


#include <vector>
#include <map>
#include <string>
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
     * Get all DesktopEntries with a given category name
     * @param category the category name
     * @return a vector of all matching DesktopEntries
     */
    std::vector<DesktopEntry> getCategoryEntries(String category);
    
    /**
     * Get the list of all categories found in all desktop entries.
     * @return a vector of all category entries
     */
    std::vector<DesktopEntry> getCategories();
    
    /**
     * Get the list of main categories specified by 
     * the desktop menu specification
     * @param excludeUnused if true, don't return categories unless
     * at least one application is using them.
     * @return the category entries
     */
    std::vector<DesktopEntry> getMainCategories(bool excludeUnused);
private:
    //list of all entries
    std::vector<DesktopEntry> entries;
    //all categories found in entries
    std::vector<DesktopEntry> categoryEntries;
    //maps category names to lists of entries
    std::map<String,std::vector<DesktopEntry>> categories;
    //stores previously searched icon paths
    PathRecord pathRecord;
 
};

#endif /* DESKTOPENTRIES_H */

