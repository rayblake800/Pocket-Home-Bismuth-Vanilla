/*
  ==============================================================================

    MainConfigFile.h
    Created: 5 Jan 2018 2:35:02pm
    Author:  anthony

  ==============================================================================
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
    static const String shutdownCommandKey;
    static const String restartCommandKey;
    static const String termLaunchCommandKey;
    Array<String> getStringKeys() const;
    //######################### Boolean Data ###################################
    //boolean value keys
    static const String showCursorKey;
    Array<String> getBoolKeys() const;

private:
    static constexpr const char* filenameConst = "config.json";
};