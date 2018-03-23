#include "AssetFiles.h"

File absoluteFileFromPath(const String& path)
{
    return File::isAbsolutePath(path) ? File(path)
            : File::getCurrentWorkingDirectory().getChildFile(path);
}

/**     
 * Loads an asset file using its asset name.
 */
File AssetFiles::findAssetFile(const String& assetName, bool lookOutsideAssets)
{
    File assetFile = absoluteFileFromPath(String(assetFolder) + assetName);
    if (!assetFile.existsAsFile() && lookOutsideAssets)
    {
        assetFile = absoluteFileFromPath(assetName);
    }
    return assetFile;
}

/**
 * Create an Image object from an asset file.
 */
Image AssetFiles::loadImageAsset(const String& assetName,
        bool lookOutsideAssets)
{
    if (assetName.endsWith(".svg"))
    {
        Image image;
        ScopedPointer<Drawable> svgDrawable
                = loadSVGDrawable(assetName, lookOutsideAssets);
        if (svgDrawable != nullptr)
        {
            image = Image(Image::ARGB, 128, 128, true);
            Graphics g(image);
            svgDrawable->drawWithin(g, Rectangle<float>(0, 0, image.getWidth(),
                                                        image.getHeight()),
                    RectanglePlacement::fillDestination, 1.0f);

        }
        return image;

    }
    File imageFile = findAssetFile(assetName, lookOutsideAssets);
    return ImageFileFormat::loadFrom(imageFile);
}

/**
 * Creates a Drawable object from an svg asset file.
 */
Drawable * AssetFiles::loadSVGDrawable(const String& assetName,
        bool lookOutsideAssets)
{
    File svgFile = findAssetFile(assetName, lookOutsideAssets);
    if (!svgFile.existsAsFile() || svgFile.getFileExtension() != ".svg")
    {
        DBG(__func__ << ": " << svgFile.getFileName() << " not found!");
        return nullptr;
    }
    ScopedPointer<XmlElement> svgElement = XmlDocument::parse(svgFile);
    if (svgElement == nullptr)
    {
        DBG(__func__ << ": " << svgFile.getFileName() << " not a valid svg!");
        return nullptr;
    }
    return Drawable::createFromSVG(*svgElement);
}

/**
 * Loads JSON data from an asset file.
 */
var AssetFiles::loadJSONAsset(const String& assetName,
        bool lookOutsideAssets)
{
    File jsonFile = findAssetFile(assetName, lookOutsideAssets);
    if (!jsonFile.existsAsFile() || jsonFile.getFileExtension() != ".json")
    {
        DBG(__func__ << ": " << jsonFile.getFileName() << " not found!");
        return var();
    }
    return JSON::parse(jsonFile);
}