/*
  ==============================================================================

    sysutils.cpp
    Created: 15 Dec 2017 6:07:53pm
    Author:  anthony

  ==============================================================================
*/

#include "sysutils.h"
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "sysutils.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <regex>
#include <locale.h>




bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string getLocale() {
    std::locale l("");
    std::string localeName=l.name();
    std::string::size_type cutIndex=localeName.find(".");
    if(cutIndex != std::string::npos)localeName.erase(cutIndex);
    return localeName;
}

std::string getTheme() {
    std::string configPath = getHomePath() + "/.gtkrc-2.0";
    if (fileExists(configPath)) {
        std::regex re("icon.*theme.*=\"(.+)\"");
        std::ifstream file(configPath);
        std::smatch match;
        for (std::string line; getline(file, line);) {
            if (std::regex_search(line, match, re)) {
                return match.str(1);
            }
        }
    }
    return "";
}

std::string getFallbackTheme() {
    std::string configPath = getHomePath() + "/.gtkrc-2.0";
    if (fileExists(configPath)) {
        std::regex re("fallback.*icon.*theme.*=\"(.+)\"");
        std::ifstream file(configPath);
        std::smatch match;
        for (std::string line; getline(file, line);) {
            if (std::regex_search(line, match, re)) {
                return match.str(1);
            }
        }
    }
    return "";
}

std::string getHomePath() {
    return std::string(std::getenv("HOME"));
}

//perform function(struct dirent*) on all files in path
void foreachFile(const std::string& path, std::function<void(struct dirent*)> fn){
    DIR * dir = opendir(path.c_str());
    if (dir != NULL) {
        struct dirent *dirdata = NULL;
        do {
            dirdata = readdir(dir);
            if (dirdata != NULL) {
                fn(dirdata);
            }
        } while (dirdata != NULL);
        closedir(dir);
    }
}

//List all non-directory files in path
std::vector<std::string> listFiles(const std::string& path){
    std::vector<std::string> files;
    foreachFile(path,[&files](struct dirent* dirdata){
        if(dirdata->d_type!=DT_DIR && dirdata->d_name!=""){
                files.push_back(dirdata->d_name);
        }
    });
    return files;
}

//list all directory files in path, ignoring ./ and ../
std::vector<std::string> listDirectoryFiles(const std::string& path){
    std::vector<std::string> directories;
    foreachFile(path,[&directories](struct dirent* dirdata){
        std::string name=dirdata->d_name;
        if(dirdata->d_type==DT_DIR 
                && name != "." && name != ".." && !name.empty()){
                directories.push_back(name);
        }
    });
    return directories;
}

