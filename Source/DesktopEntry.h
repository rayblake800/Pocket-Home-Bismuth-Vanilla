/*
  ==============================================================================

    DesktopEntry.h
    Created: 15 Dec 2017 6:08:57pm
    Author:  anthony

  ==============================================================================
*/

#pragma once


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
    DesktopEntry(std::string path, std::string localeName);
    DesktopEntry(const DesktopEntry& orig);
    virtual ~DesktopEntry();
    
    std::string getName();
    std::string getVersion();
    std::string getGenericName();
    std::string getComment();
    std::string getIconPath();
    std::string getExec();
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
};

#endif /* DESKTOPENTRY_H */

