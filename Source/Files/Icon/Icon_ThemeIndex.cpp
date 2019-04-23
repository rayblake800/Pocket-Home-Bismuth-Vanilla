#include "Icon_ThemeIndex.h"
#include <limits>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Icon::ThemeIndex::";
#endif

// Filename shared by all icon theme indexes:
static const constexpr char* indexFileName = "/index.theme";

// Creates a new ThemeIndex for a single icon theme directory.
Icon::ThemeIndex::ThemeIndex(juce::File themeDir) :
path(themeDir.getFullPathName()),
cacheFile(themeDir.getFullPathName())
{
    using juce::String;
    using juce::StringArray;
    using juce::File;
    using std::function;
    using std::map;
    if (!themeDir.isDirectory())
    {
        DBG(dbgPrefix << __func__ << ": Theme directory \""
                << themeDir.getFullPathName() << "\" does not exist.");
        path = String();
        return;
    }

    File themeIndex(themeDir.getFullPathName() + indexFileName);
    if (!themeIndex.existsAsFile())
    {
        DBG(dbgPrefix << __func__ << ": Theme directory \""
                << themeDir.getFullPathName() << "\" has no index file.");
        path = String();
        return;
    }
    StringArray fileLines = StringArray::fromLines
            (themeIndex.loadFileAsString());

    static const String themeSectionName = "Icon Theme";
    String sectionName;

    // Map each data key string to a function that saves the key's value:
    static const map<String, function<void(ThemeIndex*, String&, String&)>>
            assignmentFunctions =
    {
        {
            "Name",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->name = val;
            }
        },
        {
            "Directories",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                StringArray dirNames = StringArray::fromTokens(val, ",", "");
                for (const String& dir : dirNames)
                {
                    self->directories[dir].path = dir;
                }
            }
        },
        {
            "Comment",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->comment = val;
            }
        },
        {
            "Inherits",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->inheritedThemes = StringArray::fromTokens(val, ",", "");
            }
        },
        {
            "Hidden",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->hidden = (val == "true");
            }
        },
        {
            "Example",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->example = val;
            }
        },
        {
            "Size",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].size = val.getIntValue();
            }
        },
        {
            "Scale",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].scale = val.getIntValue();
            }
        },
        {
            "MaxSize",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].maxSize = val.getIntValue();
            }
        },
        {
            "MinSize",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].minSize = val.getIntValue();
            }
        },
        {
            "Threshold",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                self->directories[sectionName].threshold = val.getIntValue();
            }
        },
        {
            "Context",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                static const std::map<String, Context> contexts =
                {
                    {"Actions",       Context::actions},
                    {"Animations",    Context::animations},
                    {"Applications",  Context::applications},
                    {"Categories",    Context::categories},
                    {"Devices",       Context::devices},
                    {"Emblems",       Context::emblems},
                    {"Emotes",        Context::emotes},
                    {"International", Context::international},
                    {"MimeTypes",     Context::mimeTypes},
                    {"Places",        Context::places},
                    {"Status",        Context::status}
                };
                auto contextIter = contexts.find(val);
                if (contextIter != contexts.end())
                {
                    self->directories[sectionName].context
                            = contextIter->second;
                }
            }
        },
        {
            "Type",
            [] (ThemeIndex* self, String& val, String& sectionName)
            {
                static const std::map<String, SizeType> types =
                {
                    {"Fixed",     SizeType::fixed},
                    {"Scalable",  SizeType::scalable},
                    {"Threshold", SizeType::threshold}
                };
                auto typeIter = types.find(val);
                if (typeIter != types.end())
                {
                    self->directories[sectionName].type = typeIter->second;
                }
            }
        }
    };

    for (const String& line : fileLines)
    {
        if (line.startsWithChar('[') && line.endsWithChar(']'))
        {
            sectionName = line.substring(1, line.length() - 1);
        }
        else if (line.isNotEmpty())
        {
            int divider = line.indexOfChar('=');
            if (divider == -1)
            {
                continue;
            }
            String key = line.substring(0, divider);
            auto keyActionIter = assignmentFunctions.find(key);
            if (keyActionIter != assignmentFunctions.end())
            {
                String val = line.substring(divider + 1);
                keyActionIter->second(this, val, sectionName);
            }
        }
    }
}


// Checks if this object represents a valid icon theme.
bool Icon::ThemeIndex::isValidTheme() const
{
    return path.isNotEmpty();
}


// Finds the path of an icon within the theme, matching an icon name, size,
// scale factor, and context.
juce::String Icon::ThemeIndex::lookupIcon
(juce::String icon, int size, Context context, int scale) const
{
    using juce::String;
    if (!isValidTheme())
    {
        return String();
    }

    juce::Array<IconDirectory> searchDirs;

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
                // A directory referenced in the theme's icon cache file wasn't
                // defined in the theme's index file, add it as an undefined
                // IconDirectory.
                DBG(dbgPrefix << __func__ << ": Cached directory \""
                        << it->first << "\" is not present in the \""
                        << getName() << "\" theme index file, "
                        << "adding it as an undefined IconDirectory.");
                IconDirectory undefinedDir;
                undefinedDir.path = it->first;
                undefinedDir.undefined = true;
                searchDirs.add(undefinedDir);
            }
        }
    }
    else if (cacheFile.isValidCache())
    {
        // Cache is valid and doesn't contain the icon, so stop looking.
        return String();
    }
    else
    {
        // Cache is invalid, so we'll need to search all possible directories.
        for (auto dirIter = directories.begin(); dirIter != directories.end();
            dirIter++)
        {
            if (context == Context::unknown
                    || context == dirIter->second.context)
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
            if (juce::File(filePath + extension).existsAsFile())
            {
                return filePath + extension;
            }
            else
            {
                DBG(dbgPrefix << __func__ << ": Cached file is missing: "
                        << filePath << extension);
            }
        }
        catch(std::out_of_range e)
        {
            // File extensions not found, continue on to check all possible
            // extensions:
        }
        static const juce::StringArray extensions = {".png", ".svg", ".xpm"};
        for (const String& ext : extensions)
        {
            juce::File iconFile(filePath + ext);
            if (iconFile.existsAsFile())
            {
                return iconFile.getFullPathName();
            }
        }
    }
    return String();
}


// Gets the name of the icon theme.
juce::String Icon::ThemeIndex::getName() const
{
    return name;
}


// Gets a short comment describing the icon theme.
juce::String Icon::ThemeIndex::getComment() const
{
    return comment;
}


// Gets the names of all themes inherited by this icon theme.
juce::StringArray Icon::ThemeIndex::getInheritedThemes() const
{
    return inheritedThemes;
}


// Checks if this theme should be displayed to the user in theme lists.
bool Icon::ThemeIndex::isHidden() const
{
    return hidden;
}


// Gets the name of an icon to use as an example of this theme.
juce::String Icon::ThemeIndex::getExampleIcon() const
{
    return example;
}


// Compares two icon directories by their distance from the target size and
// scale.
int Icon::ThemeIndex::DirectoryComparator::compareElements
(IconDirectory first, IconDirectory second)
{
    // Always prioritize directories that were properly defined in the index
    // file:
    if (first.undefined)
    {
        return second.undefined ? 0 : 1;
    }
    else if (second.undefined)
    {
        return -1;
    }

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


// Checks if an icon directory is suitable for the target size and scale.
bool Icon::ThemeIndex::DirectoryComparator::directoryMatchesSize
(const IconDirectory& subdir)
{
    if (scale != subdir.scale)
    {
        return false;
    }
    switch(subdir.type)
    {
        case SizeType::fixed:
            return size == subdir.size;
        case SizeType::scalable:
            return size >= subdir.minSize && size <= subdir.maxSize;
        case SizeType::threshold:
            return abs(size - subdir.size) < subdir.threshold;
    }
    return false;
}


// Finds the distance between an icon directory's icon size and the target icon
// size and scale.
int Icon::ThemeIndex::DirectoryComparator::directorySizeDistance
(const IconDirectory& subdir)
{
    switch(subdir.type)
    {
        case SizeType::fixed:
            return abs(subdir.size * subdir.scale - size * scale);
        case SizeType::scalable:
            if (size * scale < subdir.minSize * subdir.scale)
            {
                return subdir.minSize * subdir.scale - size * scale;
            }
            if (size * scale > subdir.maxSize * subdir.scale)
            {
                return size * scale - subdir.maxSize * subdir.scale;
            }
            return 0;
        case SizeType::threshold:
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
    return 0;
}
