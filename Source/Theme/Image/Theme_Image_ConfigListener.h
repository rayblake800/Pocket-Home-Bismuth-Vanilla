#pragma once
/**
 * @file  Theme_Image_ConfigListener.h
 *
 * @brief  Receives updates when basic image path values set through the 
 *         Image::JSONResource change.
 */

#include "Config_Listener.h"

namespace Theme { namespace Image { class ConfigListener; } }
namespace Theme { namespace Image { class JSONResource; } }

class Theme::Image::ConfigListener : public Config::Listener<JSONResource>
{
public:
    ConfigListener();
    virtual ~ConfigListener() { }
};
