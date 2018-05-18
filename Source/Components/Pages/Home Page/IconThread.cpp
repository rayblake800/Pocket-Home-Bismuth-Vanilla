#include "AssetFiles.h"
#include "Utils.h"
#include "IconThread.h"

const String IconThread::defaultIconPath =
        "/usr/share/pocket-home/appIcons/default.png";

ReadWriteLock IconThread::iconLock;

ScopedPointer<ResourceManager::SharedResource> IconThread::sharedResource
        = nullptr;

IconThread::IconThread() :
ResourceManager(sharedResource, iconLock,
        [this]()->ResourceManager::SharedResource*
{
    return new IconResource();
})
{
    defaultIcon = AssetFiles::loadImageAsset(defaultIconPath);
}

/**
 * Queues up an icon request.  
 */
void IconThread::loadIcon(String icon, std::function<void(Image) > assignImage)
{
    //if the icon variable is a full path, return that
    if (icon.substring(0, 1) == "/")
    {
        assignImage(AssetFiles::loadImageAsset(icon));
    }
    else
    {
        //if icon is a partial path, trim it
        if (icon.contains("/"))
        {
            icon = icon.substring(1 + icon.lastIndexOf("/"));
        }
        assignImage(defaultIcon);
        const ScopedWriteLock queueLock(iconLock);
        IconResource::QueuedJob iconRequest;
        IconResource* iconResource
                = dynamic_cast<IconResource*> (sharedResource.get());
        iconRequest.icon = icon;
        iconRequest.callback = assignImage;
        iconResource->addQueuedJob(iconRequest);
        if (!iconResource->isThreadRunning())
        {
            iconResource->startThread();
        }
    }
}

IconThread::IconResource::IconResource() :
Thread("IconThread") { }

IconThread::IconResource::~IconResource()
{
    signalThreadShouldExit();
    waitForThreadToExit(-1);
}

/**
 * Returns the number of pending icon requests. 
 */
int IconThread::IconResource::numJobsQueued()
{
    return queuedJobs.size();
}

/**
 * Adds another job request to the queue.
 * 
 * @param newJob
 */
void IconThread::IconResource::addQueuedJob
(IconThread::IconResource::QueuedJob newJob)
{
    queuedJobs.add(newJob);
}

/**
 * Removes and returns the last job from the list.
 */
IconThread::IconResource::QueuedJob
IconThread::IconResource::getQueuedJob()
{
    QueuedJob lastJob = queuedJobs.getLast();
    queuedJobs.removeLast();
    return lastJob;
}

/**
 * Searches for and returns an icon.
 */
String IconThread::IconResource::getIconPath(String icon)
{
    //if it hasn't happened already, build the icon path map
    if (!iconPathsMapped)
    {
        mapIcons();
    }
    if (iconPaths.count(icon) > 0)
    {
        return iconPaths[icon];
    }
    else
    {
        // If the icon isn't in the map, look for one with a similar name
        for (auto it = iconPaths.begin(); it != iconPaths.end(); it++)
        {
            String iconCandidate = it->first;
            if (!it->second.isEmpty() &&
                (iconCandidate.containsIgnoreCase(icon) ||
                 icon.containsIgnoreCase(iconCandidate)))
            {
                return it->second;
            }
        }
    }
    return String();
}

/**
 * Removes an icon from the icon path map.
 * 
 * @param iconName  If an icon with this key is in the list of mapped
 *                   icons, it will be removed.
 */
void IconThread::IconResource::removeIcon(String iconName)
{
    auto iconIter = iconPaths.find(iconName);
    iconPaths.erase(iconIter);
}

/**
 * Assign an integer value to an icon file for sorting purposes.
 */
static int iconDirValue(const File& file)
{
    String filename = file.getFileName();
    const String numeric = "0123456789";
    if (filename.containsAnyOf(numeric))
    {
        int value = filename.substring(filename.indexOfAnyOf(numeric))
                .initialSectionContainingOnly(numeric)
                .getIntValue();
        //higher numbers first, until after 128px
        if (value > 128)
        {
            value = 128 / value;
        }
        return value;
    }
    return 0;
}

int IconThread::IconResource::IconFileComparator::compareElements
(File first, File second)
{
    if (first == second)
    {
        return 0;
    }
    return iconDirValue(second) - iconDirValue(first);
}

/**
 * While AppMenuButtons still need icons, this finds them in a separate 
 * thread.
 */
void IconThread::IconResource::run()
{
    ScopedExecTimer(String("Loading ") + String(numJobsQueued()) + " Icons");
    for(IconResource::QueuedJob activeJob = getQueuedJob();
        !threadShouldExit() && activeJob.icon.isNotEmpty();
        activeJob = getQueuedJob())
    {
        String icon = getIconPath(activeJob.icon);
        if (icon.isNotEmpty())
        {
            Image iconImg;
            {
                const MessageManagerLock lock;
                iconImg = AssetFiles::loadImageAsset(icon);
            }
            if (iconImg.isNull())
            {
                DBG("Removing unloadable icon " << icon);
                removeIcon(File(icon).getFileNameWithoutExtension());
                addQueuedJob(activeJob);
            }
            else
            {
                const MessageManagerLock lock;
                activeJob.callback(iconImg);
            }
        }
        
    }
}

/**
 * Creates the map of all icon file paths.
 */
void IconThread::IconResource::mapIcons()
{
    ScopedExecTimer(String("building icon map"));
    //Subdirectories with these names are likely to appear, but should
    //not be searched for icons.
    const StringArray ignore = {
                                "actions",
                                "applets",
                                "cursors",
                                "devices",
                                "emblems",
                                "emotes",
                                "mimetypes",
                                "places",
                                "status",
                                "stock",
                                "symbolic"
    };
    Array<File> searchDirs;
    //Recursively traverse all subdirectories at a given path, and add all valid
    //ones to the list of paths to search
    std::function<void(const File&) > findSearchPaths;
    findSearchPaths =
            [&findSearchPaths, &ignore, &searchDirs, this]
            (const File & directory)
            {
                searchDirs.add(directory);
                Array<File> searchResults = directory.findChildFiles
                        (File::findDirectories, false);
                if (searchResults.isEmpty()) return;
                Array<File> subDirs;
                for (const File& directory : searchResults)
                {
                    if (!ignore.contains(directory.getFileName(), true))
                    {
                        subDirs.add(directory);
                    }
                }
                IconFileComparator comp;
                subDirs.sort(comp);
                for (const File& directory : subDirs)
                {
                    if (!searchDirs.contains(directory))
                    {
                        findSearchPaths(directory);
                    }
                }
            };
    //check user and pocket-home directories before all else
    findSearchPaths(File("/usr/share/pocket-home/appIcons/"));
    findSearchPaths(File("~/.icons/"));
    findSearchPaths(File("~/.local/share/icons"));
    //build a list of primary search directories
    const StringArray basePaths = {
                                   "/usr/share/icons/",
                                   "/usr/local/icons/",
                                   "/usr/share/pixmaps/"
    };
    //search for icon theme directories, prioritize them if found
    String iconTheme;
    String fallbackIconTheme;
    File configPath("~/.gtkrc-2.0");
    if (configPath.existsAsFile())
    {
        String themeLine("gtk-icon-theme-name=\"");
        String fallbackThemeLine("gtk-fallback=icon-theme=\"");
        StringArray lines;
        configPath.readLines(lines);
        for (const String& line : lines)
        {
            if (iconTheme.isNotEmpty() && line.contains(themeLine))
            {
                iconTheme = line.fromFirstOccurrenceOf("\"", false, false)
                        .upToLastOccurrenceOf("\"", false, false);
            }
            if (fallbackIconTheme.isNotEmpty() && line.contains
                (fallbackThemeLine))
            {
                fallbackIconTheme = line
                        .fromFirstOccurrenceOf("\"", false, false)
                        .upToLastOccurrenceOf("\"", false, false);
            }
            if (iconTheme.isNotEmpty() && fallbackIconTheme.isNotEmpty())
            {
                break;
            }
        }
    }
    if (iconTheme.isNotEmpty())
    {
        for (const String& path : basePaths)
        {
            findSearchPaths(File(path + iconTheme + "/"));
        }
    }
    if (fallbackIconTheme.isNotEmpty())
    {
        for (const String& path : basePaths)
        {
            findSearchPaths(File(path + fallbackIconTheme + "/"));
        }
    }
    for (const String& path : basePaths)
    {
        findSearchPaths(File(path));
    }

    DBG("IconThread::" << __func__ << ":Searching " << searchDirs.size()
            << " icon directories:");
    //finally, find and map icon files
    for (const File& directory : searchDirs)
    {
        Array<File> iconFiles = directory.findChildFiles(File::findFiles, false,
                "*.png;*.svg;*.xpm;*.jpg");
        for (const File& file : iconFiles)
        {
            String filename = file.getFileNameWithoutExtension();
            if (iconPaths[filename].isEmpty())
            {
                iconPaths[filename] = file.getFullPathName();
            }
        }
    }
    DBG("IconThread::" << __func__ << ": Mapped " << String(iconPaths.size())
            << " icon files.");
    iconPathsMapped = true;
}
