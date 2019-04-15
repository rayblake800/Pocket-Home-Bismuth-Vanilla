#pragma once
/**
 * @file  Config_MainListener.h
 *
 * @brief  A listener that tracks updates in the main config.json file.
 */

#include "Config_Listener.h"

namespace Config
{
    class MainListener;
    class MainResource;
}

/**
 * @brief  Receives updates when relevant values change within the main
 *         application configuration file.
 */
class Config::MainListener : public Listener<MainResource>
{
public:
    MainListener();
    virtual ~MainListener() { }
};
