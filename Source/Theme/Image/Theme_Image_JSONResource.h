#pragma once
/**
 * @file  Theme_Image_JSONResource.h
 *
 * @brief  Reads and writes component image assets and image colors in the
 *         imageAssets.json file.
 */

#include "Config_FileResource.h"
#include "Theme_Image_AssetList.h"

namespace Theme { namespace Image { class JSONResource; } }

class Theme::Image::JSONResource : public Config::FileResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    JSONResource();

    virtual ~JSONResource() { }

    /**
     * @brief  Gets the configured image asset list for a particular component.
     * 
     * @param componentKey  A configurable UI component's key string.
     * 
     * @return              The image asset list defined for that component.
     */
    AssetList getAssetList(const juce::Identifier& componentKey) const;

private:
    /**
     * @brief  Copies all AssetList data back to the JSON file.
     */
    virtual void writeDataToJSON() final override;

    /**
     * @brief   Gets the key string and data type for each basic value stored in
     *          imageAsset.json.
     * 
     * @return  The empty list, as imageAsset.json only stores AssetList 
     *          objects.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() 
        const final override;

    /* Stores all image asset lists loaded from the image asset config file */
    std::map<juce::Identifier, AssetList> assetLists;
};

