#pragma once
/**
 * @file Config/MainListener.h
 *
 * @brief  A listener that tracks updates in the main config.json file.
 */

#include "Config/Listener.h"

namespace Config { class MainListener; }
namespace Config { class MainResource; }

class Config::MainListener : public Listener<MainResource>
{
public:
    MainListener();
    virtual ~MainListener() { }
};
