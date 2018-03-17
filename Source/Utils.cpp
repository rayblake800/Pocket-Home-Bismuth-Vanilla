
#include <sys/stat.h>
#include <dirent.h>
#include <locale.h>
#include "Utils.h"
using namespace juce;

File absoluteFileFromPath(const String &path)
{
    return File::isAbsolutePath(path) ? File(path)
            : File::getCurrentWorkingDirectory().getChildFile(path);

}

File assetFile(const String &fileName)
{
    auto devFile = absoluteFileFromPath("../../assets/" + fileName);
    File assetFile;

#if JUCE_LINUX
    // are we linux? look in /usr/share/
    // FIXME: don't hardcode this, maybe find it via .deb configuration
    File linuxAssetFile = absoluteFileFromPath("/usr/share/pocket-home/" + fileName);
    // look in relative path, used in development builds
    assetFile = linuxAssetFile.exists() ? linuxAssetFile : devFile;
    if (assetFile == devFile && !devFile.exists())
        assetFile = absoluteFileFromPath(fileName);
#else
    assetFile = devFile;
#endif

    return assetFile;
}

Image createImageFromFile(const File &imageFile)
{
    
    if (!imageFile.existsAsFile()){
        return Image();
    }
    Image image;
    if (imageFile.getFileExtension() == ".svg")
    {
        image = Image(Image::ARGB, 128, 128, true);
        ScopedPointer<Drawable> svgDrawable = createSVGDrawable(imageFile);
        Graphics g(image);
        svgDrawable->drawWithin(g, Rectangle<float>(0, 0, image.getWidth(), image.getHeight()),
                RectanglePlacement::fillDestination, 1.0f);
    } else
    {
        image = ImageFileFormat::loadFrom(imageFile);
    }
    return image;
}

Drawable * createSVGDrawable(const File& svgFile)
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

#if JUCE_DEBUG
//Print debug info about the component tree
void componentTrace()
{
    static DrawableRectangle highlightFocus;
    highlightFocus.setFill(FillType(Colour(0x0)));
    highlightFocus.setStrokeFill(FillType(Colour(0xff00ff00)));
    highlightFocus.setStrokeType(PathStrokeType(4));
    std::function<void(Component*, int) > recursiveInfo;
    recursiveInfo = [&recursiveInfo](Component* component, int depth)
    {
        String indent;
        for (int i = 0; i < depth; i++)
        {
            indent += "\t";
        }
        DBG(indent + String("Component:") + component->getName());
        indent += " ";
        DBG(indent + String("Position: (") + String(component->getX()) + String(",") +
                String(component->getY()) + String(")"));
        DBG(indent + String("Size: ") + String(component->getWidth()) + String("x") +
                String(component->getHeight()));
        String properties;
        if (component->getWantsKeyboardFocus())
        {
            properties += "wantsKeyFocus,";
        }
        if (component->hasKeyboardFocus(false))
        {
            properties += "hasKeyFocus,";
            highlightFocus.setBounds(component->getScreenBounds());
        }
        properties += component->isShowing() ? "showing" : "not showing";
        DBG(indent + properties);
        int numChildren = component->getNumChildComponents();
        if (numChildren > 0)
        {
            DBG(indent + String("Children:") + String(numChildren));
        }
        for (int i = 0; i < numChildren; i++)
        {
            recursiveInfo(component->getChildComponent(i), depth + 1);
        }
    };
    Component * rootComponent = Desktop::getInstance().getComponent(0);
    recursiveInfo(rootComponent, 0);
    rootComponent->addAndMakeVisible(highlightFocus);
}
#endif

Rectangle<int> getWindowSize()
{
    Component * windowComp = Desktop::getInstance().getComponent(0);
    if(windowComp == nullptr){
        return Rectangle<int>();
    }
    return windowComp->getLocalBounds();
}


/**
 * Requests user confirmation before performing some action
 */
void confirmAction(String title, String message, std::function<void() > onConfirm)
{
    NativeMessageBox::showOkCancelBox(AlertWindow::QuestionIcon,
            title, message, nullptr,
            ModalCallbackFunction::create([onConfirm](int response)
            {
                if (response == 1)
                {
                    onConfirm();
                }
            }));
}
