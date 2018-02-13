/**
 * @file MainConfigFile.h
 * 
 * TODO: finish documentation.
 */
#pragma once
#include "ConfigFile.h"

class MainConfigFile : public ConfigFile {
public:
    MainConfigFile();
    virtual ~MainConfigFile();

    //######################### String Data ###################################
    //string value keys
    static const String backgroundKey;
    static const String menuTypeKey;
    static const String shutdownCommandKey;
    static const String restartCommandKey;
    static const String sleepCommandKey;
    static const String termLaunchCommandKey;
    Array<String> getStringKeys() const;
    //######################### Boolean Data ###################################
    //boolean value keys
    static const String showCursorKey;
    Array<String> getBoolKeys() const;

private:
    static constexpr const char* filenameConst = "config.json";
};