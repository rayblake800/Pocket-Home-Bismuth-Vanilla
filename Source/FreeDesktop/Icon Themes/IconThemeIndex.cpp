#include <limits>
#include "IconThemeIndex.h"

IconThemeIndex::IconThemeIndex(juce::File themeDir) :
path(themeDir.getFullPathName()),
cacheFile(themeDir.getFullPathName())
{
    using namespace juce;
    if (!themeDir.isDirectory())
    {
        DBG("IconTheme::IconTheme:: Theme directory does not exist!");
        path = String();
        return;
    }

    File themeIndex(themeDir.getFullPathName() + indexFileName);
    if (!themeIndex.existsAsFile())
    {
        DBG("IconTheme::IconTheme:: Theme index does not exist!");
        path = String();
        return;
    }
    StringArray fileLines = StringArray::fromLines
            (themeIndex.loadFileAsString());

    static const String themeSectionName = "Icon Theme";
    String sectionName;

    //Map each data key string to a function that saves the key's value
    static const std::map
            <String, std::function<void(IconThemeIndex*, String&, String&) >>
            assnFns = {
        {"Name",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->name = val;
            }},
        {"Directories",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                StringArray dirNames = StringArray::fromTokens(val, ",", "");
                for (const String& dir : dirNames)
                {
                    self->directories[dir].path = dir;
                }
            }},
        {"Comment",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->comment = val;
            }},
        {"Inherits",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->inheritedThemes
                        = StringArray::fromTokens(val, ",", "");
            }},
        {"Hidden",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->hidden = (val == "true");
            }},
        {"Example",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->example = val;
            }},
        {"Size",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].size = val.getIntValue();
            }},
        {"Scale",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].scale = val.getIntValue();
            }},
        {"MaxSize",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].maxSize = val.getIntValue();
            }},
        {"MinSize",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].minSize = val.getIntValue();
            }},
        {"Threshold",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].threshold = val.getIntValue();
            }},
        {"Context",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                static const std::map<String, Context> contexts = {
                    {"Actions", actionsCtx},
                    {"Animations", animationsCtx},
                    {"Applications", applicationsCtx},
                    {"Categories", categoriesCtx},
                    {"Devices", devicesCtx},
                    {"Emblems", emblemsCtx},
                    {"Emotes", emotesCtx},
                    {"International", internationalCtx},
                    {"MimeTypes", mimeTypesCtx},
                    {"Places", placesCtx},
                    {"Status", statusCtx}
                };
                auto contextIter = contexts.find(val);
                if (contextIter != contexts.end())
                {
                    self->directories[sectionName].context 
                            = contextIter->second;
                }
            }},
        {"Type",
         [](IconThemeIndex* self, String& val, String& sectionName)
            {
                static const std::map<String, SizeType> types = {
                    {"Fixed", fixedType},
                    {"Scalable", scalableType},
                    {"Threshold", thresholdType}
                };
                auto typeIter = types.find(val);
                if (typeIter != types.end())
                {
                    self->directories[sectionName].type = typeIter->second;
                }
            }}
    };

    for (const String& line : fileLines)
    {
        if (line.startsWithChar('[') && line.endsWithChar(']'))
        {
            sectionName = line.substring(1, line.length() - 1);
            //jassert(sectionName == "Icon Theme"
            //        || directories[sectionName].path == sectionName);
        }
        else if (line.isNotEmpty())
        {
            int divider = line.indexOfChar('=');
            if (divider == -1)
            {
                continue;
            }
            String key = line.substring(0, divider);
            auto keyActionIter = assnFns.find(key);
            if (keyActionIter != assnFns.end())
            {
                String val = line.substring(divider + 1);
                keyActionIter->second(this, val, sectionName);
            }
        }
    }
}

/**
 * Checks if this object represents a valid icon theme.
 */
bool IconThemeIndex::isValidTheme() const
{
    return path.isNotEmpty();
}

/*
 * Finds the path of an icon within the theme.  The caller is responsible
 * for searching within all inherited themes if the icon is not found.
 */
juce::String IconThemeIndex::lookupIcon
(juce::String icon, int size, Context context, int scale) const
{
    using namespace juce;
    if (!isValidTheme())
    {
        return String();
    }

    Array<IconDirectory> searchDirs;

    std::map<String, String> cacheMatches;
    cacheMatches = cacheFile.lookupIcon(icon);
    if (!cacheMatches.empty())
    {
        for (auto it = cacheMatches.begin(); it != cacheMatches.end(); it++)
        {
            try
            {
                searchDirs.add(directories.at(it->first));
            }
            catch(std::out_of_range e)
            {
                DBG("IconThemeIndex::" << __func__ << ": directory "
                        << it->first << " in cache is missing from index!");
            }
        }
    }
    else if (cacheFile.isValidCache())
    {
        //cache is valid and doesn't contain the icon, no need to keep looking
        return String();
    }
    else
    {
        for (auto dirIter = directories.begin(); dirIter != directories.end();
             dirIter++)
        {
            if (context == unknownCtx || context == dirIter->second.context)
            {
                searchDirs.add(dirIter->second);
            }
        }
    }

    DirectoryComparator comp(size, scale);
    searchDirs.sort(comp);

    for (const IconDirectory& dir : searchDirs)
    {
        String filePath = path + "/" + dir.path + "/" + icon;
        try
        {
            String extension = cacheMatches.at(dir.path);
            if (File(filePath + extension).existsAsFile())
            {
                return filePath + extension;
            }
            else
            {
                DBG("Bad cache result:" << filePath << extension);
            }
        }
        catch (std::out_of_range e)
        {
            //file extensions not found, continue on to check all possibilities
        }
        //TODO: add support for the .xpm file extension, fix svg render issues
        //static const StringArray extensions = {".png", ".svg", ".xpm"};
        //for (const String& ext : extensions)
        //{
            //File iconFile(filePath + ext);
            File iconFile(filePath + ".png");
            if (iconFile.existsAsFile())
            {
                return iconFile.getFullPathName();
            }
        //}
    }
    DBG("IconThemeIndex::" << __func__ << ": No matches for " << icon
            << " in theme directory at " << path);
    return String();
}

/*
 * Gets the name of the icon theme.
 */
juce::String IconThemeIndex::getName() const
{
    return name;
}

/*
 * Gets a short comment describing the icon theme.
 */
juce::String IconThemeIndex::getComment() const
{
    return comment;
}

/*
 * Gets the names of all themes inherited by this icon theme.  When findIcon
 * doesn't locate a requested icon, all inherited themes should be searched.
 */
juce::StringArray IconThemeIndex::getInheritedThemes() const
{
    return inheritedThemes;
}

/*
 * Checks if this theme should be displayed to the user in theme lists.
 */
bool IconThemeIndex::isHidden() const
{
    return hidden;
}

/*
 * Gets the name of an icon to use as an example of this theme.
 */
juce::String IconThemeIndex::getExampleIcon() const
{
    return example;
}

/*
 * Compares two icon directories by their distance from a target size 
 * and scale.
 */
int IconThemeIndex::DirectoryComparator::compareElements
(IconDirectory first, IconDirectory second)
{
    bool firstMatches = directoryMatchesSize(first);
    bool secondMatches = directoryMatchesSize(second);

    if (firstMatches)
    {
        if (secondMatches)
        {
            return 0;
        }
        return -1;
    }
    else if (secondMatches)
    {
        return 1;
    }
    return directorySizeDistance(first) - directorySizeDistance(second);
}

/*
 * Checks if an icon directory is suitable for a given size.
 */
bool IconThemeIndex::DirectoryComparator::directoryMatchesSize
(const IconDirectory& subdir)
{
    if (scale != subdir.scale)
    {
        return false;
    }
    switch (subdir.type)
    {
        case fixedType:
            return size == subdir.size;
        case scalableType:
            return size >= subdir.minSize && size <= subdir.maxSize;
        case thresholdType:
            return abs(size - subdir.size) < subdir.threshold;
    }
    DBG("IconThemeIndex::DirectoryComparator::" << __func__ 
            << ": Missing directory type!");
    return false;
}

/*
 * Finds the distance between an icon directory's icon size and a 
 * given icon size and scale.
 */
int IconThemeIndex::DirectoryComparator::directorySizeDistance
(const IconDirectory& subdir)
{
    switch (subdir.type)
    {
        case fixedType:
            return abs(subdir.size * subdir.scale - size * scale);
        case scalableType:
            if (size * scale < subdir.minSize * subdir.scale)
            {
                return subdir.minSize * subdir.scale - size * scale;
            }
            if (size * scale > subdir.maxSize * subdir.scale)
            {
                return size * scale - subdir.maxSize * subdir.scale;
            }
            return 0;
        case thresholdType:
            if (size * scale < (subdir.size - subdir.threshold) * subdir.scale)
            {
                return (subdir.size - subdir.threshold) * subdir.scale 
                        - size * scale;
            }
            if (size * scale > (subdir.size + subdir.threshold) * subdir.scale)
            {
                return size * scale 
                        - (subdir.size + subdir.threshold)  * subdir.scale;
            }
            return 0;
    }
    DBG("IconThemeIndex::DirectoryComparator::" << __func__ 
            << ": Missing directory type!");
    return 0;
}
