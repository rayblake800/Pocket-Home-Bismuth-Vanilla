#include "AssetFiles.h"

File absoluteFileFromPath(const String& path)
{
    return File::isAbsolutePath(path) ? File(path)
            : File::getCurrentWorkingDirectory().getChildFile(path);
}

/**
 * Load an asset file using its asset name.
 * @param file the name of a file in assets, or an absolute path.
 */
File AssetFiles::findAssetFile(const String& assetName)
{
    File assetFile = absoluteFileFromPath(String(assetFolder) + assetName);
    if (!assetFile.existsAsFile())
    {
        assetFile = absoluteFileFromPath(assetName);
    }
    return assetFile;
}

/**
 * Create an Image object from an asset file
 */
Image AssetFiles::createImageAsset(const String& assetName)
{
    File imageFile = findAssetFile(assetName);
    if (!imageFile.existsAsFile())
    {
        return Image();
    }
    Image image;
    if (imageFile.getFileExtension() == ".svg")
    {
        image = Image(Image::ARGB, 128, 128, true);
        ScopedPointer<Drawable> svgDrawable = createSVGDrawable(imageFile);
        Graphics g(image);
        svgDrawable->drawWithin(g, Rectangle<float>(0, 0, image.getWidth(),
                                                    image.getHeight()),
                RectanglePlacement::fillDestination, 1.0f);
    }
    else
    {
        image = ImageFileFormat::loadFrom(imageFile);
    }
    return image;
}

/**
 * Creates a Drawable object from an svg asset file
 */
Drawable * AssetFiles::createSVGDrawable(const File& svgFile)
{
    if (!svgFile.existsAsFile())
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