#pragma once
/**
 * @file Config/MainFile.h
 * 
 * @brief Loads general application settings from the config.json file.
 */

#include "Config/FileHandler.h"
#include "Config/Listener.h"

namespace Config { class MainFile; }
namespace Config { class MainResource; }
namespace Config { class MainListener; }

class Config::MainFile : public Config::FileHandler<MainResource>
{
public:
    MainFile();
    virtual ~MainFile() { }
};
