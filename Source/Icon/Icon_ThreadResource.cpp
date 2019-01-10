#include "AssetFiles.h"
#include "Utils.h"
#include "XDGDirectories.h"
#include "IconThread.h"

const juce::Identifier IconThread::resourceKey = "IconThread";

static const constexpr char* defaultIconPath =
        "/usr/share/pocket-home/appIcons/chip.png";

IconThread::IconThread() : ThreadResource(resourceKey),
defaultIcon(AssetFiles::loadImageAsset(defaultIconPath))
{ 
    using namespace juce;
    //Icon directory search list and priority defined at
    //   https://specifications.freedesktop.org/icon-theme-spec
    //   /icon-theme-spec-latest.html
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
    iconDirectories.add("/usr/share/pocket-home/icons");
    
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
        //DBG("IconThread::IconThread: Finding icon theme " << themeNames[i]);
        for(const String& dir : iconDirectories)
        {
            File themeDir(dir + (dir.endsWithChar('/') ? "" : "/") 
                    + themeNames[i]);
            if(themeDir.isDirectory())
            {
                iconThemes.add(new IconThemeIndex(themeDir));
                if(iconThemes.getLast()->isValidTheme())
                {
                    //DBG("IconThread::IconThread: Theme directory " 
                    //        << iconThemes.size() << " added with path "
                    //        << themeDir.getFullPathName());
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
                    //DBG("IconThread::IconThread: Invalid theme directory "
                    //        << themeDir.getFullPathName());
                    iconThemes.removeLast();
                }
            }
        }
    } 
}

IconThread::~IconThread()
{
    imageCache.clear();
}

/*
 * Cancels a pending icon request.
 */
void IconThread::cancelRequest(const RequestID requestID)
{
    requestMap.erase(requestID);
}

/*
 * Adds an icon loading request to the queue.
 */
IconThread::RequestID IconThread::addRequest(IconRequest request)
{
    using std::function;
    using juce::Image;
    /* Ignore requests without valid callbacks. */
    if(!request.loadingCallback)
    {
        return 0;
    }
    /* First, attempt to load the icon from assets or the image cache. */
    Image preLoadedIcon;
    if(request.icon[0] == '/')
    {
        preLoadedIcon = AssetFiles::loadImageAsset(request.icon);
    }
    if(preLoadedIcon.isNull())
    {
        //if icon is a partial path, trim it
        if (request.icon.contains("/"))
        {
            request.icon = request.icon.substring
                (1 + request.icon.lastIndexOf("/"));
        }
        if(imageCache.contains(request.icon))
        {
            preLoadedIcon = imageCache[request.icon];
            jassert(preLoadedIcon.isValid());
        }
    }
    if(preLoadedIcon.isValid()) // Icon already found, apply now.
    {
        request.loadingCallback(preLoadedIcon);
        return 0;
    }
    else
    {
        //assign the default icon for now
        request.loadingCallback(defaultIcon);
        static RequestID newID = 0;
        while(newID == 0 || requestMap.count(newID) != 0)
        {
            newID++;
        }
        requestMap[newID] = request;
        if (!isThreadRunning())
        {
            startThread();
        }
        else //Ensure thread isn't sleeping
        {
            notify();
        }
        return newID;
    }
}


/*
 * Asynchronously handles queued icon requests.
 */
void IconThread::runLoop(ThreadLock& lock)
{
    using juce::Image;
    using juce::String;
    using std::function;
    lock.enterRead();
    RequestID requestID = requestMap.begin()->first;
    IconRequest firstRequest = requestMap[requestID];
    lock.exitRead();

    String icon = firstRequest.icon;
    String iconPath = getIconPath(firstRequest);
    if (iconPath.isNotEmpty())
    {
        Image iconImg = AssetFiles::loadImageAsset(iconPath);
        if(iconImg.isNull())
        {
            DBG("IconThread::" << __func__ << ": Unable to load icon "
                    << icon);
            return;
        }

        lock.enterWrite();
        imageCache.set(icon, iconImg);
        lock.exitWrite();

        // If callback wasn't removed, lock the message thread, check again that
        // it's still there, run the callback, and remove it from the map.
        const juce::MessageManagerLock mmLock;
        lock.enterRead();
        const bool requestPresent 
            = (requestMap.count(requestID) != 0);
        if(requestPresent)
        {
            firstRequest.loadingCallback(iconImg);
        }
        lock.exitRead();

        if(requestPresent)
        {
            lock.enterWrite();
            requestMap.erase(requestID);
            lock.exitWrite();
        }
    }
    else
    {
        // Couldn't find the icon, remove the request
        lock.enterWrite();
        requestMap.erase(requestID);
        lock.exitWrite();
    }
}

/*
 * Keeps the thread dormant when all icon requests have been processed.
 */
bool IconThread::threadShouldWait()
{
    return requestMap.empty();
}

/*
 * Search icon theme directories for an icon matching a given request.
 */
juce::String IconThread::getIconPath(const IconRequest& request)
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
        IconRequest subRequest = request;
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
        //static const StringArray iconExtensions = {".png", ".xpm", ".svg"};
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
