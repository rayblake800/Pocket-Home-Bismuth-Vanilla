#pragma once
/**
 * @file  Icon_ThemeIndex.h
 * 
 * @brief  Indexes icon directory data for a particular display theme.
 */

#include "Icon_Cache.h"
#include "Icon_Context.h"
#include "JuceHeader.h"
#include <map>

namespace Icon { class ThemeIndex; }

/**
 * @brief  Stores theme data, tracks all theme icon directories, and finds icon 
 *         paths within the icon theme.
 * 
 * For more information on icon theme specifications, see 
 * https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
 * https://specifications.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
 */
class Icon::ThemeIndex
{
public:
    /**
     * @brief  Creates a new ThemeIndex for a single icon theme directory.
     *
     * @param themeFile  The root directory of an icon theme.
     */
    ThemeIndex(juce::File themeDir);
    
    /**
     * @brief  Creates an empty, invalid index object.
     */
    ThemeIndex() { }
    
    virtual ~ThemeIndex() { }
    
    /**
     * @brief  Checks if this object represents a valid icon theme.
     *
     * @return  Whether this theme object is valid.
     */
    bool isValidTheme() const;
    
    /**
     * @brief  Defines the different size types for icon directories.
     */
    enum class SizeType
    {
        /* Icons that must be a fixed size.: */
        fixed,
        /* Icons that can be scaled to any size within a range.: */
        scalable,
        /* Icons that can can be used as long as the difference between the
         * desired size and the actual icon size is less than a threshold 
         * value: */
        threshold
    };
    
    /**
     * @brief  Finds the path of an icon within the theme, matching an icon 
     *         name, size, scale factor, and context.  
     *
     * The caller is responsible for searching within all inherited themes if 
     * the icon is not found.
     * 
     * @param icon     The name of an icon file, without the file extension.
     *
     * @param size     The desired icon width and height, in pixels.
     *
     * @param context  The context of the requested icon. If set to 
     *                 Context::unknown, the returned icon may have any context.
     * 
     * @param scale    The intended scale factor of the requested icon.
     * 
     * @return         The path of a suitable icon file, or the empty string if 
     *                 no match is found.
     */
    juce::String lookupIcon(juce::String icon, int size,
            Context context = Context::unknown, int scale = 1) const;
    
    /**
     * @brief  Gets the name of the icon theme.
     * 
     * @return  The theme's name.
     */
    juce::String getName() const;
    
    /**
     * @brief  Gets a short comment describing the icon theme.
     * 
     * @return  The theme's comment value.
     */
    juce::String getComment() const;
    
    /**
     * @brief  Gets the names of all themes inherited by this icon theme.  
     *
     * When findIcon can't locate a requested icon, all inherited themes should 
     * be searched.
     *
     * @return  The list of inherited themes, from highest to lowest priority.
     */
    juce::StringArray getInheritedThemes() const;
    
    /**
     * @brief  Checks if this theme should be hidden from the user in theme 
     *         lists.
     * 
     * @return  Whether the theme should not be shown.
     */
    bool isHidden() const;
    
    /**
     * @brief  Gets the name of an icon to use as an example of this theme.
     * 
     * @return  An icon name, to be used as the "icon" parameter in a findIcon
     *          call.
     */
    juce::String getExampleIcon() const;
    
private:
    /**
     * @brief  Describes a directory of icon files within the theme
     */
    struct IconDirectory
    {
        /* Directory path, relative to the main theme directory: */
        juce::String path;
        int size = -1;
        int scale = 1;
        Context context = Context::unknown;
        SizeType type = SizeType::threshold;
        int maxSize = -1;
        int minSize = -1;
        int threshold = 2;

        /* An undefined IconDirectory was listed in the icon cache, but not
         * found in the theme's index file. IconDirectories listed in the index
         * file will always be preferred over undefined directories. */
        bool undefined = false;
    };
    
    /**
     * @brief  Sorts icon directories by distance from a target size and scale, 
     *         from closet to farthest.
     */
    class DirectoryComparator
    {
    public:
        /**
         * @brief  Creates a DirectoryComparator to find directories closest to
         *         a specific icon size and scale.
         *
         * @param size   Ideal icon height and width in pixels.
         * 
         * @param scale  The multiplier the icon image will be scaled by when it
         *               is displayed.
         */
        DirectoryComparator(int size, int scale) :
        size(size), scale(scale) { }
        
        virtual ~DirectoryComparator() { }
        
        /**
         * @brief  Compares two icon directories by their distance from the 
         *         target size and scale.
         * 
         * @param first   The first icon directory.
         * 
         * @param second  The second icon directory.
         * 
         * @return        A value of < 0 if first is closer than second, a value
         *                of > 0 if second is closer than first, or 0 if both 
         *                directories are equally close to the desired size and 
         *                scale.
         */
        int compareElements(IconDirectory first, IconDirectory second);
    
    private:
        /**
         * @brief  Checks if an icon directory is suitable for the target size
         *         and scale.
         * 
         * @param subdir  An icon directory defined by this theme.
         * 
         * @return        Whether this directory is compatible with icons of the 
         *                given size and scale.
         */
        bool directoryMatchesSize(const IconDirectory& subdir);

        /**
         * @brief  Finds the distance between an icon directory's icon size and 
         *         the target icon size and scale.
         * 
         * @param subdir  An icon directory defined by this theme.
         * 
         * @return        Whether this directory is compatible with icons of the 
         *                target size and scale.
         */
        int directorySizeDistance(const IconDirectory& subdir);
        
        /* Ideal icon size: */
        int size = -1;
        /* Ideal icon scale: */
        int scale = 1;
    };
    
    /* The path to the theme's base directory: */
    juce::String path;
    /* The name of the icon theme: */
    juce::String name;
    /* A brief description of the icon theme: */
    juce::String comment;
    /* The list of backup themes to search for icons missing in this theme: */
    juce::StringArray inheritedThemes;
    /* Indicates if this theme should be hidden from the user in theme lists: */
    bool hidden = false;
    /* The name of an icon to use as an example of this theme's icons: */
    juce::String example;
    /* Accesses the theme's cache file, if one exists: */
    Cache cacheFile;
    /* All icon sub-directories in the theme, indexed by relative path name: */
    std::map<juce::String, IconDirectory> directories;
};
