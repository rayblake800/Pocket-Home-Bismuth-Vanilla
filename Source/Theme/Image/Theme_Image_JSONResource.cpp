#include "Theme_Image_JSONResource.h"
#include "Theme_Image_JSONKeys.h"

namespace ThemeImages = Theme::Image;
namespace ComponentKeys = Theme::Image::JSONKeys;

/* SharedResource object key */
const juce::Identifier ThemeImages::JSONResource::resourceKey
        = "Theme_Image_JSONResource";

/* Filename of the JSON configuration file */
static const constexpr char * configFilename = "imageAssets.json";

ThemeImages::JSONResource::JSONResource() : 
Config::FileResource(resourceKey, configFilename)
{
    using juce::DynamicObject;
    using juce::Identifier;
    const juce::Array<Identifier> keys = ComponentKeys::components;
    for (const Identifier& key : keys)
    {
	    DynamicObject::Ptr listData = initProperty<DynamicObject*>(key);
        assetLists[key] = AssetList(listData);
    }
    loadJSONData();
}

/*
 * Gets the configured settings for a particular component.
 */
ThemeImages::AssetList ThemeImages::JSONResource::getAssetList
(const juce::Identifier& componentKey) const
{
    return assetLists.at(componentKey);
}

/*
 * Copies all ComponentSettings data back to the JSON file.
 */
void ThemeImages::JSONResource::writeDataToJSON()
{
    using juce::DynamicObject;
    using juce::Identifier;
    const juce::Array<Identifier>& keys = ComponentKeys::components;
    for (const Identifier& key : keys)
    {
        if(assetLists.count(key) != 0)
        {
            updateProperty<DynamicObject*>(key, 
                    assetLists[key].getDynamicObject());
        }
    }
}

/*
 * Gets the key string and data type for each basic value stored in 
 * imageResource.json.
 */
const std::vector<Config::DataKey>& ThemeImages::JSONResource::getConfigKeys()
        const  
{
    // imageResource.json doesn't have any basic values, it only holds 
    // ComponentSettings objects.
    static const std::vector<Config::DataKey> emptyList;
    return emptyList;
}
