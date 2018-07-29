/**
 * @file AssetFiles.h
 * 
 * AssetFiles loads files from the asset folder or from absolute paths,
 * and initializes file resource objects from those files.
 */
#pragma once
#include "JuceHeader.h"

namespace AssetFiles
{

    /**
     * Loads an asset file using its asset name.
     * 
     * @param assetName          The name of a file in the asset folder.
     * 
     * @param lookOutsideAssets  If the file isn't found in the asset folder,
     *                            and this value is set to true, findAssetFile
     *                            will attempt to treat assetName as a path, and
     *                            load the file from elsewhere in the file 
     *                            system.
     * 
     * @return                   the requested file, or an invalid file if 
     *                            nothing was found.
     */
    juce::File findAssetFile(const juce::String& assetName, 
            bool lookOutsideAssets = true);

    /**
     * Creates an Image object from an asset file.
     * 
     * @param assetName          The name of an image file in the asset folder.
     * 
     * @param lookOutsideAssets  If the image isn't found in the asset folder,
     *                            and this value is set to true, findAssetFile
     *                            will attempt to treat assetName as a path, and
     *                            load the image from elsewhere in the file 
     *                            system.
     * 
     * @return                   the requested file's Image, or Image() if no 
     *                            valid image file was found.
     */
    juce::Image loadImageAsset(const juce::String& assetName, 
            bool lookOutsideAssets = true);
    

    /**
     * Creates a Drawable object from an svg asset file.
     * 
     * @param svgFile            The name of a .svg file in the asset folder.
     * 
     * @param lookOutsideAssets  If the .svg isn't found in the asset folder,
     *                            and this value is set to true, findAssetFile
     *                            will attempt to treat assetName as a path, and
     *                            load the .svg from elsewhere in the file 
     *                            system.
     * 
     * @return                   A new drawable if the file was valid, nullptr 
     *                            otherwise.
     */
    juce::Drawable * loadSVGDrawable(const juce::String& assetName, 
            bool lookOutsideAssets = true);
    
    /**
     * Loads JSON data from an asset file.
     * 
     * @param assetName          The name of a .json file in the asset folder.
     * 
     * @param lookOutsideAssets  If the .json isn't found in the asset folder,
     *                            and this value is set to true, findAssetFile
     *                            will attempt to treat assetName as a path, and
     *                            load the .json from elsewhere in the file 
     *                            system.
     * 
     * @return                   A new drawable if the file was valid, nullptr 
     *                            otherwise.
     */
    juce::var loadJSONAsset(const juce::String& assetName, 
            bool lookOutsideAssets = true);

    // FIXME: don't hardcode this, maybe find it via .deb configuration
    static const constexpr char* assetFolder = "/usr/share/pocket-home/";

}