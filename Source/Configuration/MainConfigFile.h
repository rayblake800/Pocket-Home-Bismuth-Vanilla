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

    
    //string value keys
    static const String backgroundKey;
    static const String menuTypeKey;
    static const String shutdownCommandKey;
    static const String restartCommandKey;
    static const String sleepCommandKey;
    static const String termLaunchCommandKey;
    //boolean value keys
    static const String showCursorKey;
    std::vector<DataKey> getDataKeys() const;
    
    //All possible values of menuTypeKey
    static const Array<String> menuTypes;

private:
    static CriticalSection mainConfigLock;
    static constexpr const char* filenameConst = "config.json";
};
