#pragma once
#include "FileHandler.h"
#include "MainResource.h"

/**
 * @file MainFile.h
 * 
 * @brief Loads general application settings from the config.json file.
 */
class Config::MainFile : public Config::FileHandler<MainResource>
{
public:
    MainFile() { }
    virtual ~MainFile() { }

    class Listener : public Config::FileResource::Listener
    {
    public:
        Listener() : Config::FileResource::Listener(MainResource::resourceKey,
                []()->SharedResource* { return new MainResource(); }) { }
        
        virtual ~Listener() { }
    };
};
