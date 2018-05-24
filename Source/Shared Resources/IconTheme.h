#pragma once
#include <map>
#include "JuceHeader.h"

/**
 * @file IconTheme.h
 * 
 * @brief Holds icon data for a particular display theme.
 * 
 * This stores theme data, tracks all theme icon directories, and finds icon
 * paths within the icon theme.
 */

class IconTheme
{
public:
    /**
     * @param themeFile  The index.theme file defining an icon theme.
     */
    IconTheme(File themeFile);
    
    virtual ~IconTheme() { }
    
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
            int scale = 1);
    
    /**
     * Gets the name of the icon theme.
     * 
     * @return the theme's name.
     */
    String getName();
    
    /**
     * Gets a short comment describing the icon theme.
     * 
     * @return  the theme's comment value.
     */
    String getComment();
    
    /**
     * Gets the names of all themes inherited by this icon theme.  When findIcon
     * doesn't locate a requested icon, all inherited themes should be searched.
     */
    StringArray getInheritedThemes();
    
    /**
     * Checks if this theme should be displayed to the user in theme lists.
     * 
     * @return  true iff the theme should be shown.
     */
    bool isHidden();
    
    /**
     * Gets the name of an icon to use as an example of this theme.
     * 
     * @return  an icon name, to be used as the "icon" parameter in a findIcon
     *          call.
     */
    String getExampleIcon();
    
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
     * Checks if an icon directory is suitable for a given size.
     * 
     * @param subdir  An icon directory defined by this theme.
     * 
     * @param size    The intended icon size.
     * 
     * @param scale   The intended icon scale.
     * 
     * @return  true iff this directory is compatible with icons of the given
     *          size and scale.
     */
    bool directoryMatchesSize(IconDirectory subdir, int size, int scale);
        
    /**
     * Finds the distance between an icon directory's icon size and a 
     * given icon size and scale.
     * 
     * @param subdir  An icon directory defined by this theme.
     * 
     * @param size    The intended icon size.
     * 
     * @param scale   The intended icon scale.
     * 
     * @return  true iff this directory is compatible with icons of the given
     *          size and scale.
     */
    int directorySizeDistance(IconDirectory subdir, int size, int scale);
    
    String path;
    String name;
    String comment;
    StringArray inheritedThemes;
    bool hidden = false;
    String example;
    
    Array<IconDirectory> directories;
    
    
};