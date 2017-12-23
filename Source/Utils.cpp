#include "Utils.h"
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <regex>
#include <locale.h>

File absoluteFileFromPath(const String &path) {
    return File::isAbsolutePath(path) ? File(path)
            : File::getCurrentWorkingDirectory().getChildFile(path);

}

//Function to create or load the writable config file

File assetConfigFile(const String &fileName) {
    File linuxAssetFile = absoluteFileFromPath("~/.pocket-home/" + fileName);
    return linuxAssetFile;
}


// TODO: allow user overrides of asset files

File assetFile(const String &fileName) {
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

Image createImageFromFile(const File &imageFile) {
    auto image = Image(Image::ARGB, 128, 128, true);
    if (imageFile.getFileExtension() == ".svg") {
        ScopedPointer<XmlElement> svgElement = XmlDocument::parse(imageFile);
        ScopedPointer<Drawable> svgDrawable = Drawable::createFromSVG(*svgElement);
        Graphics g(image);
        svgDrawable->drawWithin(g, Rectangle<float>(0, 0, image.getWidth(), image.getHeight()),
                RectanglePlacement::centred, 1.0f);
    } else {
        image = ImageFileFormat::loadFrom(imageFile);
    }
    return image;
}

ImageButton *createImageButton(const String &name, const File &imageFile) {
    auto image = createImageFromFile(imageFile);
    return createImageButton(name, image);
}

ImageButton *createImageButton(const String &name, const Image &image) {
    auto imageButton = new ImageButton("Back");
    // FIXME: to support touch areas of different size from the base image,
    // we need to explicitly size the images within image buttons when necessary,
    // rather than relying on the resizing parameters used here in setImages().
    // Otherwise images are forced to resize whenever we change component size.
    imageButton->setImages(true, true, true,
            image, 1.0f, Colours::transparentWhite, // normal
            image, 1.0f, Colours::transparentWhite, // over
            image, 0.5f, Colours::transparentWhite, // down
            0);
    return imageButton;
}

ImageButton *createImageButtonFromDrawable(const String &name, const Drawable &drawable) {
    auto image = Image(Image::RGB, 128, 128, true);
    Graphics g(image);
    drawable.drawWithin(g, Rectangle<float>(0, 0, image.getWidth(), image.getHeight()),
            RectanglePlacement::fillDestination, 1.0f);
    return createImageButton(name, image);
}

void fitRectInRect(Rectangle<int> &rect, int x, int y, int width, int height,
        Justification justification, const bool onlyReduceInSize) {
    // it's no good calling this method unless both the component and
    // target rectangle have a finite size.
    jassert(rect.getWidth() > 0 && rect.getHeight() > 0 && width > 0 && height > 0);

    if (rect.getWidth() > 0 && rect.getHeight() > 0 && width > 0 && height > 0) {
        int newW, newH;

        if (onlyReduceInSize && rect.getWidth() <= width && rect.getHeight() <= height) {
            newW = rect.getWidth();
            newH = rect.getHeight();
        } else {
            const double imageRatio = rect.getHeight() / (double) rect.getWidth();
            const double targetRatio = height / (double) width;

            if (imageRatio <= targetRatio) {
                newW = width;
                newH = jmin(height, roundToInt(newW * imageRatio));
            } else {
                newH = height;
                newW = jmin(width, roundToInt(newH / imageRatio));
            }
        }

        if (newW > 0 && newH > 0)
            rect = justification.appliedToRectangle(Rectangle<int>(newW, newH),
                Rectangle<int>(x, y, width, height));
    }
}

void fitRectInRect(Rectangle<int> &rect, const Rectangle<int> &container,
        Justification justification, const bool onlyReduceInSize) {
    fitRectInRect(rect, container.getX(), container.getY(), container.getWidth(),
            container.getHeight(), justification, onlyReduceInSize);
}

float smoothstep(float edge0, float edge1, float x) {
    x = std::min(std::max((x - edge0) / (edge1 - edge0), 0.0f), 1.0f);
    return x * x * (3.0f - 2.0f * x);
}

float mix(float a, float b, float t) {
    return t * (b - a) + a;
}

float mapLinear(float x, float imin, float imax, float omin, float omax) {
    return mix(omin, omax, (x - imin) / (imax - imin));
}

void animateTranslation(Component *component, int x, int y, float alpha, int durationMillis) {
    const auto &bounds = component->getBounds();
    auto destBounds = bounds.translated(x - bounds.getX(), y - bounds.getY());
    Desktop::getInstance().getAnimator().animateComponent(component, destBounds, alpha,
            durationMillis, true, 0, 0);
}

std::vector<String> split(const String &orig, const String &delim) {
    std::vector<String> elems;
    int index = 0;
    auto remainder = orig.substring(index);
    while (remainder.length()) {
        index = remainder.indexOf(delim);
        if (index < 0) {
            elems.push_back(remainder);
            break;
        }
        elems.push_back(remainder.substring(0, index));
        remainder = remainder.substring(index + 1);
    }
    return elems;
};

bool fileExists(const String& path) {
    struct stat buffer;
    return (stat(path.toRawUTF8(), &buffer) == 0);
}

String getLocale() {
    std::locale l("");
    std::string localeName = l.name();
    std::string::size_type cutIndex = localeName.find(".");
    if (cutIndex != std::string::npos)localeName.erase(cutIndex);
    return String(localeName);
}

String getTheme() {
    std::string configPath = getHomePath().toStdString() + "/.gtkrc-2.0";
    if (fileExists(configPath)) {
        std::regex re("icon.*theme.*=\"(.+)\"");
        std::ifstream file(configPath);
        std::smatch match;
        for (std::string line; getline(file, line);) {
            if (std::regex_search(line, match, re)) {
                return String(match.str(1));
            }
        }
    }
    return "";
}

String getFallbackTheme() {
    std::string configPath = getHomePath().toStdString() + "/.gtkrc-2.0";
    if (fileExists(configPath)) {
        std::regex re("fallback.*icon.*theme.*=\"(.+)\"");
        std::ifstream file(configPath);
        std::smatch match;
        for (std::string line; getline(file, line);) {
            if (std::regex_search(line, match, re)) {
                return String(match.str(1));
            }
        }
    }
    return "";
}

String getHomePath() {
    return String(std::getenv("HOME"));
}

//perform function(struct dirent*) on all files in path

void foreachFile(const String& path, std::function<void(struct dirent*) > fn) {
    DIR * dir = opendir(path.toRawUTF8());
    if (dir != NULL) {
        struct dirent *dirdata = NULL;
        do {
            dirdata = readdir(dir);
            if (dirdata != NULL) {
                fn(dirdata);
            }
        } while (dirdata != NULL);
        closedir(dir);
    }
}

//List all non-directory files in path

std::vector<String> listFiles(const String& path) {
    std::vector<String> files;
    foreachFile(path, [&files](struct dirent * dirdata) {
        if (dirdata->d_type != DT_DIR && dirdata->d_name != "") {
            files.push_back(dirdata->d_name);
        }
    });
    return files;
}

//list all directory files in path, ignoring ./ and ../

std::vector<String> listDirectoryFiles(const String& path) {
    std::vector<String> directories;
    foreachFile(path, [&directories](struct dirent * dirdata) {
        String name = dirdata->d_name;
        if (dirdata->d_type == DT_DIR
                && name != "." && name != ".." && !name.isEmpty()) {
            directories.push_back(name);
        }
    });
    return directories;
}

PathRecord::PathRecord() {
    String path = getHomePath() + recordPath;
    if (fileExists(path)) {
        std::ifstream file(path.toStdString());
        std::regex re("([a-zA-Z0-9_@]+)\\=(.*)");
        std::smatch match;
        for (std::string line; getline(file, line);) {
            if (std::regex_search(line, match, re)) {
                String key = match.str(1);
                String val = match.str(2);
                if (key == "THEME") {
                    iconTheme = val;
                    if (iconTheme != getTheme()) {
                        pathRecords.clear();
                        break;
                    }
                } else {
                    pathRecords[key] = val;
                }
            }
        }
        file.close();
    } else {
        DBG(String("PathRecord:didn't find record file at ")+path);
        iconTheme = getTheme();
        changesWritten = false;
    }
}

PathRecord::PathRecord(const PathRecord& orig) {
    iconTheme = orig.iconTheme;
    pathRecords = orig.pathRecords;

}

PathRecord::~PathRecord() {
    if (!changesWritten)writeRecords();
}

String PathRecord::getRecord(String iconName){
    try{
        return pathRecords.at(iconName);
    }catch(std::out_of_range e){
        return "";
    }
}
void PathRecord::addRecord(String iconName, String fullPath){
    pathRecords[iconName]=fullPath;
    changesWritten=false;
}

void PathRecord::writeRecords() {
    String path = getHomePath() + recordPath;
    if(changesWritten)return;
    try {
        std::ofstream file(path.getCharPointer(),
                std::ofstream::out | std::ofstream::trunc);
        file << "THEME=" + iconTheme + "\n";
        for (std::map<String, String>::iterator it = pathRecords.begin();
                it != pathRecords.end(); it++) {
            file << it->first + "=" + it->second + "\n";
        }
        changesWritten = true;
        file.close();
        DBG(String("PathRecord:wrote icon cache to ")+path);
    } catch (std::ofstream::failure e) {
        DBG("Couldn't write to path record file!");
    }
}