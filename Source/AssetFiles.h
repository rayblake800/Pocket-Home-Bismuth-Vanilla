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
     * Load an asset file using its asset name.
     * @param assetName the name of a file in assets, or an absolute path.
     * @return the requested file, or an invalid file if nothing was found.
     */
    File findAssetFile(const String& assetName);

    /**
     * Create an Image object from an asset file
     * @param assetName the name of an image file in assets, or the absolute 
     *  path of an image file.
     * @return the requested file's Image, or Image() if no valid image file
     * was found
     */
    Image createImageAsset(const String& assetName);
    

    /**
     * Creates a Drawable object from an svg asset
     * @param svgFile a valid svg asset file name.
     * @return A drawable if the file was valid, nullptr otherwise
     */
    Drawable * createSVGDrawable(const File&  assetFile);

#if JUCE_LINUX
    // FIXME: don't hardcode this, maybe find it via .deb configuration
    const constexpr char* assetFolder = "/usr/share/pocket-home/";
#else
    const constexpr char* assetFolder = "../../assets/";
#endif
}