#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_JSONResource.h"
#include "Theme_Image_JSONKeys.h"
#include "Theme_Image_AssetList.h"

Theme::Image::ConfigFile::ConfigFile() { }

/*
 * Gets a configured image asset list from shared JSON file data.
 */
Theme::Image::AssetList Theme::Image::ConfigFile::getAssetList
(const juce::Identifier& componentKey) const
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();
    return config->getAssetList(componentKey);
}

/*
 * Gets the path to the HomePage background image.
 */
juce::String Theme::Image::ConfigFile::getHomeBackgroundPath() const
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();
    return config->getConfigValue<juce::String>(JSONKeys::homeBackground);
}

/*
 * Gets the path to the LoginPage background image.
 */
juce::String Theme::Image::ConfigFile::getLoginBackgroundPath() const
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();
    return config->getConfigValue<juce::String>(JSONKeys::loginBackground);
}

/*
 * Gets the path to the default AppMenu icon image.
 */
juce::String Theme::Image::ConfigFile::getDefaultIconPath() const
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();
    return config->getConfigValue<juce::String>(JSONKeys::defaultIcon);
}

/*
 * Gets the path to the image displayed in the center of the LoginPage.
 */
juce::String Theme::Image::ConfigFile::getLoginImagePath() const
{
    SharedResource::LockedPtr<const JSONResource> config 
            = getReadLockedResource();
    return config->getConfigValue<juce::String>(JSONKeys::loginImage);
}

/*
 * Sets the path to the HomePage background image.
 */
void Theme::Image::ConfigFile::setHomeBackgroundPath
(const juce::String newPath)
{
    SharedResource::LockedPtr<JSONResource> config = getWriteLockedResource();
    config->setConfigValue(JSONKeys::homeBackground, newPath);
}

/*
 * Sets the path to the LoginPage background image.
 */
void Theme::Image::ConfigFile::setLoginBackgroundPath
(const juce::String newPath)
{
    SharedResource::LockedPtr<JSONResource> config = getWriteLockedResource();
    config->setConfigValue(JSONKeys::loginBackground, newPath);
}

/*
 * Sets the path to the default AppMenu icon image.
 */
void Theme::Image::ConfigFile::setDefaultIconPath(const juce::String newPath)
{
    SharedResource::LockedPtr<JSONResource> config = getWriteLockedResource();
    config->setConfigValue(JSONKeys::defaultIcon, newPath);
}

/*
 * Sets the path to the image displayed in the center of the LoginPage.
 */
void Theme::Image::ConfigFile::setLoginImagePath(const juce::String newPath)
{
    SharedResource::LockedPtr<JSONResource> config = getWriteLockedResource();
    config->setConfigValue(JSONKeys::loginImage, newPath);
}
