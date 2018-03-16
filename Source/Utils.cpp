
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

float smoothstep(float edge0, float edge1, float x)
{
    x = std::min(std::max((x - edge0) / (edge1 - edge0), 0.0f), 1.0f);
    return x * x * (3.0f - 2.0f * x);
}

float mix(float a, float b, float t)
{
    return t * (b - a) + a;
}

float mapLinear(float x, float imin, float imax, float omin, float omax)
{
    return mix(omin, omax, (x - imin) / (imax - imin));
}

void animateTranslation(Component *component, int x, int y, float alpha, int durationMillis)
{
    const auto &bounds = component->getBounds();
    auto destBounds = bounds.translated(x - bounds.getX(), y - bounds.getY());
    Desktop::getInstance().getAnimator().animateComponent(component, destBounds, alpha,
            durationMillis, true, 0, 0);
}

Array<String> split(const String &orig, const String &delim)
{
    Array<String> elems;
    String remainder = orig;
    while (remainder.isNotEmpty())
    {
        int index = remainder.indexOf(delim);
        if (index == -1)
        {
            elems.add(remainder);
            break;
        }
        elems.add(remainder.substring(0, index));
        remainder = remainder.substring(index + 1);
    }
    return elems;
};

String getHomePath()
{
    return String(std::getenv("HOME"));
}

//perform function(struct dirent*) on all files in path

void foreachFile(const String& path, std::function<void(struct dirent*) > fn)
{
    DIR * dir = opendir(path.toRawUTF8());
    if (dir != nullptr)
    {
        struct dirent *dirdata = nullptr;
        do
        {
            dirdata = readdir(dir);
            if (dirdata != nullptr)
            {
                fn(dirdata);
            }
        } while (dirdata != nullptr);
        closedir(dir);
    }
}

//List all non-directory files in path

std::vector<String> listFiles(const String& path)
{
    std::vector<String> files;
    foreachFile(path, [&files](struct dirent * dirdata)
    {
        if (dirdata->d_type != DT_DIR && dirdata->d_name != "")
        {
            files.push_back(dirdata->d_name);
        }
    });
    return files;
}

//list all directory files in path, ignoring ./ and ../

std::vector<String> listDirectoryFiles(const String& path)
{
    std::vector<String> directories;
    foreachFile(path, [&directories](struct dirent * dirdata)
    {
        String name = dirdata->d_name;
        if (dirdata->d_type == DT_DIR
                && name != "." && name != ".." && !name.isEmpty())
        {
            directories.push_back(name);
        }
    });
    return directories;
}

#if JUCE_DEBUG
//Print debug info about the component tree
void componentTrace()
{
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
            highlightFocus.setBounds(component->getBounds());
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

//resizes a font to fit in a containing rectangle.
//If fitting it in would require mangling the font size too much, the
//font gets set to size zero.

Font fontResizedToFit(Font font, String text, Rectangle<int>container)
{
    float currentHeight = font.getHeight();
    float currentWidth = font.getStringWidth(text);
    int newHeight = currentHeight * container.getWidth() / currentWidth;
    if (newHeight > container.getHeight())
    {
        newHeight = container.getHeight();
    }
    font.setHeight(newHeight);
    return font;
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
