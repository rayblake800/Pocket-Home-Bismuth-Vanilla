/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sysutils.h
 * Author: anthony
 *
 * Created on December 15, 2017, 1:56 AM
 */

#ifndef SYSUTILS_H
#define SYSUTILS_H

#include <string>
#include <functional>
#include <vector>

bool fileExists(const std::string& path);
std::string getLocale();
std::string getTheme();
std::string getFallbackTheme();
std::string getHomePath();

//perform function(struct dirent*) on all files in path
void foreachFile(const std::string& path, std::function<void(struct dirent*)> fn);

//List all non-directory files in path
std::vector<std::string> listFiles(const std::string& path);

//list all directory files in path, ignoring ./ and ../
std::vector<std::string> listDirectoryFiles(const std::string& path);

/**
for each entry in container, run fn(entry)
if fn returns true, stop the loop
 * container: must have a .size method, and
 * data entries accessible using container[index]
 * 
 * fn:must be some form of callable function that accepts an element
 * of the type held in c, and returns a boolean value.
**/
template<class C,class F>
void foreach(C container,F fn){
    for(int i = 0; i<container.size();i++){
        if(fn(container[i]))break;
    }
}

/**
 * Split string s into vector entries around char c
 */
std::vector<std::string> split(std::string s,char c);
#endif /* SYSUTILS_H */

