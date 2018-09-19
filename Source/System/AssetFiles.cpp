#include "AssetFiles.h"

juce::File absoluteFileFromPath(const juce::String& path)
{
    using namespace juce;
    return File::isAbsolutePath(path) ? File(path)
            : File::getCurrentWorkingDirectory().getChildFile(path);
}

/**     
 * Loads an asset file using its asset name.
 */
juce::File AssetFiles::findAssetFile
(const juce::String& assetName, bool lookOutsideAssets)
{
    using namespace juce;
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
juce::Image AssetFiles::loadImageAsset
(const juce::String& assetName, bool lookOutsideAssets)
{
    using namespace juce;
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
juce::Drawable * AssetFiles::loadSVGDrawable
(const juce::String& assetName, bool lookOutsideAssets)
{
    using namespace juce;
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
juce::var AssetFiles::loadJSONAsset
(const juce::String& assetName, bool lookOutsideAssets)
{
    using namespace juce;
    File jsonFile = findAssetFile(assetName, lookOutsideAssets);
    if (!jsonFile.existsAsFile() || jsonFile.getFileExtension() != ".json")
    {
        DBG(__func__ << ": " << jsonFile.getFileName() << " not found!");
        return var();
    }
    return JSON::parse(jsonFile);
}