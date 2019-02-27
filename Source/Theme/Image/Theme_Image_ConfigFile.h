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

    /**
     * @brief  Gets the path to the HomePage background image.
     *
     * @return  The HopePage background path. This may be either a global path,
     *          or relative to the application's assets folder. This may be
     *          the empty string.
     */
    juce::String getHomeBackgroundPath() const;

    /**
     * @brief  Gets the path to the LoginPage background image.
     *
     * @return  The LoginPage background path. This may be either a global path,
     *          or relative to the application's assets folder. This may be
     *          the empty string.
     */
    juce::String getLoginBackgroundPath() const;

    /**
     * @brief  Gets the path to the default AppMenu icon image.
     *
     * @return  The default icon path. This may be either a global path,
     *          or relative to the application's assets folder. This may be
     *          the empty string.
     */
    juce::String getDefaultIconPath() const;

    /**
     * @brief  Gets the path to the image displayed in the center of the 
     *         LoginPage.
     *
     * @return  The LoginPage path. This may be either a global path,
     *          or relative to the application's assets folder. This may be
     *          the empty string.
     */
    juce::String getLoginImagePath() const;

    /**
     * @brief  Sets the path to the HomePage background image.
     *
     * @param newPath  The new image path, either global or relative to the
     *                 assets directory, or the empty string to disable this
     *                 image.
     */
    void setHomeBackgroundPath(const juce::String newPath);

    /**
     * @brief  Sets the path to the LoginPage background image.
     *
     * @param newPath  The new image path, either global or relative to the
     *                 assets directory, or the empty string to disable this
     *                 image.
     */
    void setLoginBackgroundPath(const juce::String newPath);

    /**
     * @brief  Sets the path to the default AppMenu icon image.
     *
     * @param newPath  The new image path, either global or relative to the
     *                 assets directory, or the empty string to disable this
     *                 image.
     */
    void setDefaultIconPath(const juce::String newPath);

    /**
     * @brief  Sets the path to the image displayed in the center of the
     *         LoginPage.
     *
     * @param newPath  The new image path, either global or relative to the
     *                 assets directory, or the empty string to disable this
     *                 image.
     */
    void setLoginImagePath(const juce::String newPath);
};
