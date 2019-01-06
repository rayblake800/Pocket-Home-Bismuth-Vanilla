#pragma once
/**
 * @file  Theme_Image_ConfigFile.h
 * 
 * @brief  Gets and sets the component settings held by the 
 *         Theme::Image::JSONResource.
 */

#include "Config_FileHandler.h"

namespace Theme { namespace Image { class ConfigFile; } }
namespace Theme { namespace Image { class JSONResource; } }
namespace Theme { namespace Image { class AssetList; } }

/** 
 *  The ConfigFile reads and writes settings defined in 
 * Theme::Image::JSONResource's configurable JSON file. This includes lists of
 * image assets, along with lists of colors to apply to those image assets.
 */
class Theme::Image::ConfigFile : public Config::FileHandler<JSONResource>
{
public:
    ConfigFile();

    virtual ~ConfigFile() { }

    /**
     * @brief   Gets a configured image asset list from shared JSON file data.
     * 
     * @param componentKey  A configurable UI component's key string.
     * 
     * @return              The image asset list defined for that component.
     */
    AssetList getAssetList(const juce::Identifier& componentKey) const;
};
