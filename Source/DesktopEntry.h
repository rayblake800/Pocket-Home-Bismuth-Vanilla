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
    static constexpr const char* DEFAULT_APP_ICON_PATH=
    "/usr/share/pocket-home/appIcons/default.png";
    static constexpr const char* DEFAULT_DIRECTORY_ICON_PATH=
    "/usr/share/pocket-home/appIcons/filebrowser.png";
    //Contains filename(no extention)=fullpath/filename.extension mappings
    //for all icons found on the system.  Recursively mapping all icon
    //directories is time consuming and should only be done once per run.
    static std::map<String,String> iconPaths;
    static bool iconPathsMapped;
    //path of the .Desktop/.Directory file
    String entrypath;
    //keys that store string and boolean data:
    std::map<String, String> appStrings;
    std::map<String,bool> appBools;
    Type type;
    //initialize string and bool maps with all valid keys
    void mapInit();
    //used to track down absolute icon paths
    void mapIcons(); 
};

#endif /* DESKTOPENTRY_H */

