#pragma once
#include <map>
#include "IconCache.h"
#include "JuceHeader.h"

/**
 * @file IconThemeIndex.h
 * 
 * @brief Holds icon data for a particular display theme.
 * 
 * This stores theme data, tracks all theme icon directories, and finds icon
 * paths within the icon theme.
 * 
 * For more information on icon theme specifications, see 
 * https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
 * https://specifications.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
 */

class IconThemeIndex
{
public:
    /**
     * @param themeFile  The root directory of an icon theme.
     */
    IconThemeIndex(File themeDir);
    
    /**
     * Creates an empty, invalid index object.
     */
    IconThemeIndex() { }
    
    virtual ~IconThemeIndex() { }
    
    /**
     * Checks if this object represents a valid icon theme.
     */
    bool isValidTheme() const;
    
    /**
     * Defines the purpose of an icon directory.
     */
    enum Context
    {
        //Icons used in menus and dialogs for user interaction
        actionsCtx,
        //Animated images used to represent background processing
        animationsCtx,
        //Icons representing individual applications
        applicationsCtx,
        //Icons representing application categories
        categoriesCtx,
        //Icons representing hardware devices
        devicesCtx,
        //Icons for tags and file properties (e.g. "read-only", "photos")
        emblemsCtx,
        //Icons for emoticons used in text/messenger applications
        emotesCtx,
        //Different national flags, etc.
        internationalCtx,
        //Icons for specific data types (e.g. "audio", "image")
        mimeTypesCtx,
        //Icons representing file locations
        placesCtx,
        //Icons for presenting statuses to the user, such as error icons.
        statusCtx,
        //Icon directory has a missing or unexpected context value
        unknownCtx
    };
    
    /**
     * Defines the different size types for icon directories.
     */
    enum SizeType
    {
        //icons that must be a fixed size.
        fixedType,
        //icons that can be scaled to any size within a range.
        scalableType,
        //icons that can can be used as long as the difference between the
        //desired size and the actual icon size is less than a threshold value.
        thresholdType
    };
    
    /**
     * Finds the path of an icon within the theme, matching an icon name, size,
     * scale factor, and context.  The caller is responsible for searching 
     * within all inherited themes if the icon is not found.
     * 
     * @param icon     The name of an icon file, without the file extension.
     *
     * @param size     The desired icon width and height, in pixels.
     *
     * @param context  The context of the requested icon. If set to unknownCtx,
     *                 the returned icon may have any context.
     * 
     * @param scale    The intended scale factor of the requested icon.
     * 
     * @return  the path of a suitable icon file, or the empty string if no
     *          match is found.
     */
    String lookupIcon(String icon, int size, Context context = unknownCtx,
            int scale = 1) const;
    
    /**
     * Gets the name of the icon theme.
     * 
     * @return the theme's name.
     */
    String getName() const;
    
    /**
     * Gets a short comment describing the icon theme.
     * 
     * @return  the theme's comment value.
     */
    String getComment() const;
    
    /**
     * Gets the names of all themes inherited by this icon theme.  When findIcon
     * doesn't locate a requested icon, all inherited themes should be searched.
     */
    StringArray getInheritedThemes() const;
    
    /**
     * Checks if this theme should be displayed to the user in theme lists.
     * 
     * @return  true iff the theme should be shown.
     */
    bool isHidden() const;
    
    /**
     * Gets the name of an icon to use as an example of this theme.
     * 
     * @return  an icon name, to be used as the "icon" parameter in a findIcon
     *          call.
     */
    String getExampleIcon() const;
    
private:
    
    /**
     * Describes a directory of icon files within the theme
     */
    struct IconDirectory
    {
        //Directory path, relative to the main theme directory
        String path;
        
        int size = -1;
        int scale = 1;
        Context context = unknownCtx;
        SizeType type = thresholdType;
        int maxSize = -1;
        int minSize = -1;
        int threshold = 2;
    };
    
    /**
     * Sorts icon directories by distance from a target size and scale, from
     * closet to farthest.
     */
    class DirectoryComparator
    {
    public:
        /**
         * @param size   Ideal icon height and width in pixels.
         * 
         * @param scale  The multiplier the icon image will be scaled by when it
         *               is displayed.
         */
        DirectoryComparator(int size, int scale) :
        size(size), scale(scale) { }
        
        virtual ~DirectoryComparator() { }
        
        /**
         * Compares two icon directories by their distance from a target size 
         * and scale.
         * 
         * @param first  The first icon directory.
         * 
         * @param second The second icon directory.
         * 
         * @return a value of < 0 if first is closer than second,
         *         a value of > 0 if second is closer than first,
         *         or 0 if both directories are equally close to the desired
         *         size and scale.
         */
        int compareElements(IconDirectory first, IconDirectory second);
    
    private:
        
        /**
         * Checks if an icon directory is suitable for a given size.
         * 
         * @param subdir  An icon directory defined by this theme.
         * 
         * @return  true iff this directory is compatible with icons of the 
         *          given size and scale.
         */
        bool directoryMatchesSize(const IconDirectory& subdir);

        /**
         * Finds the distance between an icon directory's icon size and a 
         * given icon size and scale.
         * 
         * @param subdir  An icon directory defined by this theme.
         * 
         * @return  true iff this directory is compatible with icons of the 
         *          given size and scale.
         */
        int directorySizeDistance(const IconDirectory& subdir);
        
        int size = -1;
        int scale = 1;
    };
    
    //The path to the theme's base directory
    String path;
    //The name of the icon theme
    String name;
    //A brief description of the icon theme
    String comment;
    //The list of backup themes to search for icons missing in this theme
    StringArray inheritedThemes;
    //Indicates if this theme should be shown to the user in theme lists
    bool hidden = false;
    //The name of an icon to use as an example of this theme's icons
    String example;
    //Accesses the theme's cache file, if one exists
    IconCache cacheFile;
    //Filename shared by all icon theme indexes
    static const constexpr char* indexFileName = "/index.theme";
    
    //All icon sub-directories in the theme, indexed by relative path name
    std::map<String, IconDirectory> directories;
    
    
};