#include "AssetFiles.h"
#include "Utils.h"
#include "XDGDirectories.h"
#include "IconThread.h"

const juce::String IconThread::defaultIconPath =
        "/usr/share/pocket-home/appIcons/default.png";

juce::ReadWriteLock IconThread::iconLock;

juce::ScopedPointer<ResourceManager::SharedResource> IconThread::sharedResource
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

/*
 * Queues up an icon request.  
 */
void IconThread::loadIcon(
        juce::String icon,
        int size,
        std::function<void(juce::Image) > assignImage,
        IconThemeIndex::Context context,
        int scale)
{
    using namespace juce;
    //DBG("IconThread::" << __func__ << ": Requesting icon " << icon 
    //        << ", target size " << size);
    //if the icon variable is a full path, return that
    if (icon.substring(0, 1) == "/")
    {
        assignImage(AssetFiles::loadImageAsset(icon));
    }
    else
    {
        //assign the default icon for now
        assignImage(defaultIcon);
        //if icon is a partial path, trim it
        if (icon.contains("/"))
        {
            icon = icon.substring(1 + icon.lastIndexOf("/"));
        }
        const ScopedWriteLock queueLock(iconLock);
        IconResource* iconResource
                = dynamic_cast<IconResource*> (sharedResource.get());
        IconResource::QueuedJob iconRequest = {
            icon,
            size,
            scale,
            context,
            assignImage
        };
        iconResource->addQueuedJob(iconRequest);
        if (!iconResource->isThreadRunning())
        {
            iconResource->startThread();
        }
    }
}

IconThread::IconResource::IconResource() :
Thread("IconThread") 
{ 
    using namespace juce;
    //Icon directory search list and priority defined at
    //https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html
    iconDirectories.add(String(getenv("HOME")) + "/.icons");
    StringArray dataDirs = XDGDirectories::getDataSearchPaths();
    for(String& dir : dataDirs)
    {
        if(!dir.endsWithChar('/'))
        {
            dir += '/';
        }
        dir += "icons";
    }
    iconDirectories.addArray(dataDirs);
    iconDirectories.add("/usr/share/pixmaps");
    iconDirectories.add("/usr/share/pocket-home/appIcons");
    
    StringArray themeNames;
    //Icon theme selection is stored in $HOME/.gtkrc-2.0
    StringArray themeSettings;
    String gtkSettingPath(String(getenv("HOME")) + "/.gtkrc-2.0");
    File(gtkSettingPath).readLines(themeSettings);
    for(const String& line : themeSettings)
    {
        int divider = line.indexOfChar('=');
        if(divider != -1)
        {
            String key = line.substring(0,divider);
            if(key == "gtk-icon-theme-name" || key == "gtk-fallback-icon-theme")
            {
                themeNames.add(line.substring(divider+1).unquoted());
                if(themeNames.size() > 1)
                {
                    break;
                }
            }
        }
    }
    themeNames.add("hicolor");//The default fallback theme
    
    //create theme index objects for the user's icon theme and all
    //inherited/fallback themes
    for(int i = 0; i < themeNames.size(); i++)
    {
        DBG("IconThread::IconThread: Finding icon theme " << themeNames[i]);
        for(const String& dir : iconDirectories)
        {
            File themeDir(dir + (dir.endsWithChar('/') ? "" : "/") 
                    + themeNames[i]);
            if(themeDir.isDirectory())
            {
                iconThemes.add(new IconThemeIndex(themeDir));
                if(iconThemes.getLast()->isValidTheme())
                {
                    DBG("IconThread::IconThread: Theme directory " 
                            << iconThemes.size() << " added with path "
                            << themeDir.getFullPathName());
                    StringArray inherited = iconThemes.getLast()
                            ->getInheritedThemes();
                    int insertParentIdx = i + 1;
                    for(const String& parent : inherited)
                    {
                        if(!themeNames.contains(parent))
                        {
                            themeNames.insert(insertParentIdx, parent);
                            insertParentIdx++;
                        }
                    }
                    break;
                }
                else
                {
                    DBG("IconThread::IconThread: Invalid theme directory "
                            << themeDir.getFullPathName());
                    iconThemes.removeLast();
                }
            }
        }
    } 
}

IconThread::IconResource::~IconResource()
{
    signalThreadShouldExit();
    waitForThreadToExit(-1);
}

/*
 * Returns the number of pending icon requests. 
 */
int IconThread::IconResource::numJobsQueued()
{
    return queuedJobs.size();
}

/*
 * Adds another job request to the queue.
 */
void IconThread::IconResource::addQueuedJob
(IconThread::IconResource::QueuedJob newJob)
{
    queuedJobs.add(newJob);
}

/*
 * Removes and returns the last job from the list.
 */
IconThread::IconResource::QueuedJob
IconThread::IconResource::getQueuedJob()
{
    QueuedJob lastJob = queuedJobs.getLast();
    queuedJobs.removeLast();
    return lastJob;
}

/*
 * While AppMenuButtons still need icons, this finds them in a separate 
 * thread.
 */
void IconThread::IconResource::run()
{
    using namespace juce;
    for(IconResource::QueuedJob activeJob = getQueuedJob();
        !threadShouldExit() && activeJob.icon.isNotEmpty();
        activeJob = getQueuedJob())
    {
        String icon = getIconPath(activeJob);
        if (icon.isNotEmpty())
        {
            Image iconImg;
            {
                const MessageManagerLock lock;
                iconImg = AssetFiles::loadImageAsset(icon);
            }
            if (!iconImg.isNull())
            {
                const MessageManagerLock lock;
                activeJob.callback(iconImg);
            }
        }
        
    }
}

/*
 * Search icon theme directories for an icon matching a given request.
 */
juce::String IconThread::IconResource::getIconPath
(const IconThread::IconResource::QueuedJob& request)
{
    using namespace juce;
    //First, search themes in order to find a matching icon
    for(const IconThemeIndex* themeIndex : iconThemes)
    {
        String iconPath = themeIndex->lookupIcon(request.icon, request.size,
                request.context, request.scale);
        if(iconPath.isNotEmpty())
        {
            return iconPath;
        }
    }
    //If not searching within the application context and the icon name is
    //hyphenated, remove the last section of the name to search for a less
    //specific icon.
    if(request.context != IconThemeIndex::applicationsCtx &&
       request.icon.containsChar('-'))
    {
        QueuedJob subRequest = request;
        subRequest.icon = subRequest.icon.upToLastOccurrenceOf("-", false,
                false);
        String iconPath = getIconPath(subRequest);
        if(iconPath.isNotEmpty())
        {
            return iconPath;
        }
    }
    //Last, search for matching un-themed icon files
    for(const String& iconDir : iconDirectories)
    {
        //TODO: add support for .xpm files, fix svg rendering problems
        static const StringArray iconExtensions = {".png", ".xpm", ".svg"};
        //for(const String& ext : iconExtensions)
        //{
            static const char* ext = ".png";
            String iconPath = iconDir + String("/") + request.icon + ext;
            if(File(iconPath).existsAsFile())
            {
                return iconPath;
            }
        //}
    }
    return String();
}
