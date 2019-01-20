#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_JSONResource.h"
#include "Theme_Image_AssetList.h"

namespace ThemeImages = Theme::Image;

ThemeImages::ConfigFile::ConfigFile() { }

/*
 * Gets a configured image asset list from shared JSON file data.
 */
ThemeImages::AssetList ThemeImages::ConfigFile::getAssetList
(const juce::Identifier& componentKey) const
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();
    return config->getAssetList(componentKey);
}
