#pragma once
/**
 * @file  Theme_Image_AssetList.h
 *
 * @brief  Holds a single Component's image resources and image colors.
 */

#include "JuceHeader.h"

namespace Theme { namespace Image { class AssetList; } }

class Theme::Image::AssetList
{
public:
    /**
     * @brief  Creates an empty image asset list object.
     */
    AssetList();

    /**
     * @brief  Initializes the asset list from JSON data.
     * 
     * @param jsonObj  A dynamic object containing JSON data.
     */
    AssetList(juce::DynamicObject* jsonObj);

    /**
     * @brief  Packages the asset list into a juce::DynamicObject.
     * 
     * @return  The converted asset list.
     */
    juce::DynamicObject * getDynamicObject() const;
    
    /**
     * @brief   Gets the configurable list of image files.
     *
     * @return  The list of component image asset files.
     */
    const juce::StringArray& getImageFiles() const;

    /**
     * @brief  Gets the list of configurable color values.
     *
     * @return  The list of configurable image color values.
     */
    const juce::Array<juce::Colour>& getImageColours() const;

    /**
     * @brief  Compares this object with another AssetList object.
     *
     * @param rhs  Another image asset object to compare with this one.
     *
     * @return     Whether both objects define the same list of image assets and
     *             image colors in the same order.
     */
    bool operator==(const AssetList& rhs) const;

private:
    /* Component image asset file names or paths. */
    juce::StringArray imageFiles;

    /* Custom colors for component image assets. */
    juce::Array<juce::Colour> colours;
};
