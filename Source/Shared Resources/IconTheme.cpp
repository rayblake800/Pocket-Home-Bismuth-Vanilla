#include <limits>
#include "IconTheme.h"

IconTheme::IconTheme(File themeFile)
{
    jassert(themeFile.existsAsFile()
            && themeFile.getFileName() == "index.theme");
    path = themeFile.getParentDirectory().getFullPathName();
    StringArray fileLines = StringArray::fromLines
            (themeFile.loadFileAsString());

    static const String themeSectionName = "Icon Theme";
    String sectionName;
    IconDirectory currentDir;
      
    static const std::map<String, 
            std::function<void(IconTheme*, String&, IconDirectory&)>> assnFns =
    {
        {"Name",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    self->name = val;
                }},
        {"Comment",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    self->comment = val;
                }},
        {"Inherits",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    self->inheritedThemes
                            = StringArray::fromTokens(val, ",", "");
                }},
        {"Hidden",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    self->hidden = (val == "true");
                }},
        {"Example",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    self->example = val;
                }},
        {"Size",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    dir.size = val.getIntValue();
                }},
        {"Scale",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    dir.scale = val.getIntValue();
                }},
        {"MaxSize",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    dir.maxSize = val.getIntValue();
                }},
        {"MinSize",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    dir.minSize = val.getIntValue();
                }},
        {"Threshold",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    dir.threshold = val.getIntValue();
                }},
        {"Context",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    static const std::map<String, Context> contexts=
                    {
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
                    if(contextIter != contexts.end())
                    {
                        dir.context = contextIter->second;
                    }
                }},
        {"Type",
                [](IconTheme* self, String& val, IconDirectory& dir)
                {
                    static const std::map<String, SizeType> types =
                    {
                        {"Fixed", fixedType},
                        {"Scalable", scalableType},
                        {"Threshold", thresholdType}
                    };
                    auto typeIter = types.find(val);
                    if(typeIter != types.end())
                    {
                        dir.type = typeIter->second;
                    }
                }}
    };

    for (const String& line : fileLines)
    {
        if (line.startsWithChar('[') && line.endsWithChar(']'))
        {
            if (sectionName.isNotEmpty() && sectionName != themeSectionName)
            {
                currentDir.path = sectionName;
                directories.add(currentDir);
                currentDir = IconDirectory();
            }
            sectionName = line.substring(1, line.length() - 2);
        }
        else if (line.isNotEmpty())
        {
            int divider = line.indexOfChar('=');
            if (divider == -1)
            {
                jassertfalse;
                continue;
            }
            String key = line.substring(0, divider);
            auto keyActionIter = assnFns.find(key);
            if(keyActionIter != assnFns.end())
            {
                String val = line.substring(divider);
                keyActionIter->second(this,val,currentDir);
            }
        }
    }
    directories.add(currentDir);
}

/*
 * Finds the path of an icon within the theme.  The caller is responsible
 * for searching within all inherited themes if the icon is not found.
 */
String IconTheme::lookupIcon(String icon, int size, Context context, int scale) 
{
    static const StringArray extensions = { ".png", ".svg", ".xpm"};
    for(const IconDirectory& dir : directories)
    {
        if((context == unknownCtx || dir.context == context)
            && directoryMatchesSize(dir, size, scale))
        {
            for(const String& ext : extensions)
            {
                File iconFile(path + dir.path + icon + ext);
                if(iconFile.existsAsFile())
                {
                    return iconFile.getFullPathName();
                }
            }   
        }
    }
    int minDistance = std::numeric_limits<int>::max() ;
    String closestFilename;
    for(const IconDirectory& dir : directories)
    {
       if(context == unknownCtx || dir.context == context)
        {
            for(const String& ext : extensions)
            {
                File iconFile(path + dir.path + icon + ext);
                if(iconFile.existsAsFile())
                {
                    int distance = directorySizeDistance(dir, size, scale);
                    if(distance < minDistance)
                    {
                        distance = minDistance;
                        closestFilename = iconFile.getFullPathName();
                    }
                }
            }   
        } 
    }
    return closestFilename;
}

/*
 * Gets the name of the icon theme.
 */
String IconTheme::getName() 
{ 
    return name;
}

/*
 * Gets a short comment describing the icon theme.
 */
String IconTheme::getComment() 
{ 
    return comment;
}

/*
 * Gets the names of all themes inherited by this icon theme.  When findIcon
 * doesn't locate a requested icon, all inherited themes should be searched.
 */
StringArray IconTheme::getInheritedThemes() 
{ 
    return inheritedThemes;
}

/*
 * Checks if this theme should be displayed to the user in theme lists.
 */
bool IconTheme::isHidden() 
{ 
    return hidden;
}

/*
 * Gets the name of an icon to use as an example of this theme.
 */
String IconTheme::getExampleIcon() 
{ 
    return example;
}

/*
 * Checks if an icon directory is suitable for a given size.
 */
bool IconTheme::directoryMatchesSize(IconDirectory subdir, int size, int scale) 
{ 
    if(scale != subdir.scale)
    {
        return false;
    }
    switch(subdir.type)
    {
        case fixedType:
            return size == subdir.size;
        case scalableType:
            return size >= subdir.minSize && size <= subdir.maxSize;
        case thresholdType:
            return abs(size - subdir.size) < subdir.threshold;       
    }
}

/*
 * Finds the distance between an icon directory's icon size and a 
 * given icon size and scale.
 */
int IconTheme::directorySizeDistance(IconDirectory subdir, int size, int scale) 
{ 
    switch(subdir.type)
    {
        case fixedType:
            return abs(subdir.size * subdir.scale - size * scale);
        case scalableType:
            if(size * scale < subdir.minSize * subdir.scale)
            {
                return subdir.minSize * subdir.scale - size * scale;
            }
            if(size * scale > subdir.maxSize * subdir.scale)
            {
                return size * scale - subdir.maxSize * subdir.scale;
            }
            return 0;
        case thresholdType:
            if(size * scale < (subdir.size - subdir.threshold) * subdir.scale)
            {
                return subdir.minSize * subdir.scale - size * scale;
            }
            if(size * scale > (subdir.size + subdir.threshold) * subdir.scale)
            {
                return size * scale - subdir.maxSize * subdir.scale;
            }
            return 0;
    }
}
