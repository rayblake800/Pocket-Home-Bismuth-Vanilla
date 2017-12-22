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
#include "../JuceLibraryCode/JuceHeader.h"

class DesktopEntry {
public:
    enum Type {
        APPLICATION,
        LINK,
        DIRECTORY
    };
    DesktopEntry();
    /**
     * Load DesktopEntry data from a .desktop or .directory file
     * @param path absolute path of the source file
     * @param localeName used for selecting locale-specific data from the file
     */
    DesktopEntry(String path, String localeName);
    
    /**
     * Creates a DesktopEntry object representing an application category
     * @param category the category name
     */
    DesktopEntry(String category);
    
    DesktopEntry(const DesktopEntry& orig);
    virtual ~DesktopEntry();
    
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
    //path of the .Desktop file
    String entrypath;
    //keys that store string data:
    std::map<String, String> appStrings;
    //keys that store boolean data:
    std::map<String,bool> appBools;
    //initialize string and bool maps with all valid keys
    void mapInit();
    Type type;
    //stores icon path
    String iconPath;
    //util functions to track down absolute icon paths
    String findIconPath();
    String searchIconPaths(String icon, String path); 
};

#endif /* DESKTOPENTRY_H */

