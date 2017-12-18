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

#include <string>
#include <vector>
#include <map>

class DesktopEntry {
public:
    enum Type {
        APPLICATION,
        LINK,
        DIRECTORY
    };
    DesktopEntry();
    DesktopEntry(std::string path, std::string localeName);
    DesktopEntry(const DesktopEntry& orig);
    virtual ~DesktopEntry();
    
    std::string getName();
    std::string getVersion();
    std::string getGenericName();
    std::string getComment();
    std::string getIconPath();
    std::string getExec();
    std::string getPath();
    std::string getStartupWMClass();
    std::string getURL();
    std::vector<std::string> getActions();
    std::vector<std::string> getMimeType();
    std::vector<std::string> getCategories();
    std::vector<std::string> getImplements();
    std::vector<std::string> getKeywords();
    
    bool onlyShowIn(std::string env);
    bool notShowIn(std::string env);
    bool tryExec();
    
    bool noDisplay();
    bool hidden();
    bool dBusActivatable();
    bool terminal();
    bool startupNotify();
private:
    //path of the .Desktop file
    std::string entrypath;
    //keys that store string data:
    std::map<std::string, std::string> appStrings;
    //keys that store boolean data:
    std::map<std::string,bool> appBools;
    Type type;
    //stores icon path
    std::string iconPath;
    //util functions to track down absolute icon paths
    std::string findIconPath();
    std::string searchIconPaths(std::string icon, std::string path); 
};

#endif /* DESKTOPENTRY_H */

