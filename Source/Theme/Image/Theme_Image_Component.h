#pragma once
/**
 * @file  Theme_Image_Component.h
 * 
 * @brief  A Component template class with image assets and image colors set in 
 *         the image assets JSON file.
 */

#include "Theme_Image_AssetList.h"
#include "Theme_Image_ConfigFile.h"
#include "DrawableImageComponent.h"

namespace Theme { namespace Image { template <class ComponentType> 
    class Component; } }

/**
 * @brief  A subclass of DrawableImageComponent with image assets and colors
 *         defined by an AssetList object.
 *
 * @tparam DrawableImageType  The Component's parent class. This should be 
 *                            either DrawableImageComponent or one of its 
 *                            subclasses.
 */
template <class DrawableImageType = DrawableImageComponent>
class Theme::Image::Component : public DrawableImageType
{
public:
    /**
     * @brief  Loads the image asset list on creation.
     *
     * @param componentKey  One of the component keys defined in
     *                      Theme::Image::JSONKeys.
     * 
     * @param assetIndex    Selects which component asset file should be loaded 
     *                      on initialization.
     * 
     * @param placement     Defines how the image will be scaled.
     */
    Component(const juce::Identifier& componentKey,
            const int assetIndex = 0,
            const juce::RectanglePlacement placement 
                    = juce::RectanglePlacement::centred) :
    DrawableImageType(placement),
    assetIndex(assetIndex) 
    {
        const ConfigFile configReader;
        assetList = configReader.getAssetList(componentKey);
        applyImageAssets();
    }


    virtual ~Component() { }

    /**
     * @brief  Loads a new image from a different asset file.
     * 
     * @param index  The index of an asset file defined for this component in
     *               its AssetList. If index is out of bounds or it equals the 
     *               current loaded index, nothing will happen. 
     */
    void setImageAssetIndex(const int index)
    {
        if (index >= 0 && assetIndex != index)
        {
            assetIndex = index;
        }
        applyImageAssets();
    }

protected:
    /**
     * @brief  Reloads the image file and sets the image colors.
     */
    virtual void applyImageAssets()
    {
        const juce::StringArray& filenames = assetList.getImageFiles();
        if (assetIndex >= 0 && assetIndex < filenames.size())
        {
            DrawableImageType::setImage(filenames[assetIndex]);
        }
        else
        {
            DBG("ThemeImages::Component::" << __func__
                    << ": couldn't load asset index " << assetIndex
                    << ", asset count is " << filenames.size());
            assetIndex = -1;
        }
        
        const juce::Array<juce::Colour>& colours = assetList.getImageColours();
        for (int i = 0; i < colours.size(); i++)
        {
            int colourId = DrawableImageType::imageColour0Id + i;
            DrawableImageType::setColour(colourId, colours[i]);
        }
    }

private:
    /* current selected image asset index. */
    int assetIndex = 0;

    /* The list of available image assets, and custom image colors. */
    AssetList assetList;
};
