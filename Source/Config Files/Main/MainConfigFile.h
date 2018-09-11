#pragma once
#include "MainJSON.h"
#include "ConfigFile.h"

/**
 * @file MainConfigFile.h
 * 
 * @brief Loads general application settings from the config.json file.
 */

class MainConfigFile : public ConfigFile<MainJSON>
{
public:
    MainConfigFile() { }

    virtual ~MainConfigFile() { }

    class Listener : public ConfigJSON::Listener
    {
    public:
        Listener() : ConfigJSON::Listener(MainJSON::resourceKey,
                []()->SharedResource* { return new MainJSON(); }) { }
        
        virtual ~Listener() { }
    };
};
