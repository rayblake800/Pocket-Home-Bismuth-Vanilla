/* 
 * File:   DesktopEntry.h
 * Author: anthony
 *
 * Created on December 14, 2017, 1:46 PM
 * 
 * Reads in standardized .Desktop file data
 * the Desktop entry format is very well documented at
 * https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html#recognized-keys
 */

#ifndef DESKTOPENTRY_H
#define DESKTOPENTRY_H
#include <map>
#include "Utils.h"
#include "../JuceLibraryCode/JuceHeader.h"

class DesktopEntry {
public:
    enum Type {
        APPLICATION,
        LINK,
        DIRECTORY
    };
    /**
     * Load DesktopEntry data from a .desktop or .directory file
     * @param path absolute path of the source file
     * @param localeName used for selecting locale-specific data from the file
     * @param pathRecords points to an icon path object held by the 
     * object that created this DesktopEntry
     */
    DesktopEntry(String path, String localeName,PathRecord * pathRecords);
    
    /**
     * Creates a DesktopEntry object representing an application category
     * @param category the category name
     * @param pathRecords points to an icon path object held by the 
     * object that created this DesktopEntry
     */
    DesktopEntry(String category,PathRecord * pathRecords);
    
    DesktopEntry(const DesktopEntry& orig);
    virtual ~DesktopEntry();
    
    //The following methods get Desktop file variables as they are
    //defined by the standard.
    
    Type getType();
    String getName();
    String getVersion();
    String getGenericName();
    String getComment();
    String getIconPath();
    String getExec();
    String getPath();
    String getStartupWMClass();
    String getURL();
    std::vector<String> getActions();
    std::vector<String> getMimeType();
    std::vector<String> getCategories();
    std::vector<String> getImplements();
    std::vector<String> getKeywords();
    

    bool onlyShowIn(String env);
    bool notShowIn(String env);
    bool tryExec();
    
    bool noDisplay();
    bool hidden();
    bool dBusActivatable();
    bool terminal();
    bool startupNotify();
private:
    //path of the .Desktop/.Directory file
    String entrypath;
    //keys that store string and boolean data:
    std::map<String, String> appStrings;
    std::map<String,bool> appBools;
    //initialize string and bool maps with all valid keys
    void mapInit();
    Type type;
    //stores icon path
    String iconPath;
    //used to track down absolute icon paths
    String findIconPath();
    String searchIconPaths(String icon, String path); 
    PathRecord * pathRecord=NULL;
};

#endif /* DESKTOPENTRY_H */

