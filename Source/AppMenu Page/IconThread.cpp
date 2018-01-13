/*
  ==============================================================================

    IconThread.cpp
    Created: 11 Jan 2018 9:33:33pm
    Author:  anthony

  ==============================================================================
 */

#include <fstream>
#include <set>
#include <regex>
#include "../Utils.h"
#include "IconThread.h"


const String IconThread::defaultAppIconPath =
        "/usr/share/pocket-home/appIcons/default.png";

const String IconThread::defaultDirectoryIconPath =
        "/usr/share/pocket-home/appIcons/filebrowser.png";

IconThread::IconThread() : Thread("IconThread")
{
    defaultAppIcon = createImageFromFile(File(defaultAppIconPath));
    defaultDirectoryIcon = createImageFromFile(File(defaultDirectoryIconPath));
}

IconThread::~IconThread()
{
}

void IconThread::loadIcon(AppMenuButton::Ptr button, String icon)
{
    if (button->isFolder())
    {
        button->appIcon = defaultDirectoryIcon;
    } else
    {
        button->appIcon = defaultAppIcon;
    }
    const ScopedLock queueLock(lock);
    QueuedJob iconRequest;
    iconRequest.button = button;
    iconRequest.icon = icon;
    queuedJobs.add(iconRequest);
    if (!isThreadRunning())
    {
        startThread();
    }
}

void IconThread::run()
{
    const ScopedLock queueLock(lock);
    while (!threadShouldExit() && queuedJobs.size() > 0)
    {
        QueuedJob activeJob = queuedJobs[0];
        queuedJobs.remove(0);
        String fullPath;
        String icon = activeJob.icon;

        //if the icon variable is a full path, return that
        if (icon.substring(0, 1) == "/")
        {
            fullPath = icon;
        } else
        {
            //if icon is a partial path, trim it
            if (icon.contains("/"))
            {
                icon = icon.substring(1 + icon.lastIndexOf("/"));
            }

            //if it hasn't happened already, build the icon path map
            if (!iconPathsMapped)
            {
                const ScopedUnlock mapUnlock(lock);
                mapIcons();
            }
            fullPath = iconPaths[icon];

            // If the icon isn't in the map, look for one with a similar name
            if (fullPath.isEmpty())
            {
                for (std::map<String, String>::iterator it = iconPaths.begin();
                        it != iconPaths.end(); it++)
                {
                    String iconCandidate = it->first;
                    if (!it->second.isEmpty() &&
                            (iconCandidate.containsIgnoreCase(icon) ||
                            icon.containsIgnoreCase(iconCandidate)))
                    {
                        iconPaths[icon] = it->second;
                        fullPath = it->second;
                        break;
                    }
                }
            }
        }
        if (fullPath.isNotEmpty())
        {
            //load icon into button asynchronously.
            MessageManager::callAsync([fullPath, activeJob]
            {
                activeJob.button->appIcon = createImageFromFile(File(fullPath));
                activeJob.button->repaint();
            });
        }
        //Allow other threads to run
        {
            const ScopedUnlock yieldUnlock(lock);
            Thread::yield();
        }
    }
}

/**
 * Creates the map of all icon file paths.
 */
void IconThread::mapIcons()
{
    //Subdirectories with these names are likely to appear, but should
    //not be searched for icons.
    std::set<String> ignore = {
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
    Array<String> searchPaths;
    //check user and pocket-home directories before all else
    searchPaths.add("/usr/share/pocket-home/appIcons/");
    searchPaths.add(getHomePath() + "/.icons/");
    searchPaths.add(getHomePath() + "/.local/share/icons");
    //Recursively traverse all subdirectories at a given path, and add all valid
    //ones to the list of paths to search
    std::function<void(String) > findSearchPaths;
    findSearchPaths = [&findSearchPaths, &ignore, &searchPaths, this]
            (String path)
    {
        std::vector<String> dirs = listDirectoryFiles(path);
        if (dirs.empty())return;
        //sort icon size directories, if found
        std::regex sizePattern("^([0-9]+)");
        std::smatch sizeMatch;
        std::sort(dirs.begin(), dirs.end(), [&sizeMatch, &sizePattern]
        (const String& a, const String & b)->bool
        {
            std::string a_str = a.toStdString();
            std::string b_str = b.toStdString();
            if (a == b)return false;
                    int aVal = 0;
                    int bVal = 0;
                if (std::regex_search(a_str, sizeMatch, sizePattern))
                {
                    sscanf(sizeMatch.str(1).c_str(), "%d", &aVal);
                }
            if (std::regex_search(b_str, sizeMatch, sizePattern))
            {
                sscanf(sizeMatch.str(1).c_str(), "%d", &bVal);
            }
            if (aVal != bVal)
            {
                //higher numbers first, until after 128px
                if (aVal > 128 && aVal < 999)aVal = 1 + 128 / aVal;
                    if (bVal > 128 && bVal < 999)bVal = 1 + 128 / bVal;
                        return aVal > bVal;
                    }
            return false;
        });
        for (const String& subDir : dirs)
        {
            //only add directories outside of the ignore set
            if (ignore.find(subDir) == ignore.end())
            {
                String subPath = path + subDir + "/";
                if (!searchPaths.contains(subPath))
                {
                    searchPaths.add(subPath);
                    findSearchPaths(subPath);
                }
            }
        };
        iconPathsMapped = true;

    };

    //build a list of primary search directories
    std::vector<String> checkPaths;
    std::vector<String> basePaths = {
        "/usr/share/icons/",
        "/usr/local/icons/",
        "/usr/share/pixmaps/"
    };
    //search for icon theme directories, prioritize them if found
    String iconTheme;
    String fallbackIconTheme;
    std::string configPath = getHomePath().toStdString() + "/.gtkrc-2.0";
    if (fileExists(configPath))
    {
        std::regex themeMatch("gtk-icon-theme-name=\"(.+)\"");
        std::regex fallbackThemeMatch("gtk-fallback=icon-theme=\"(.+)\"");
        try
        {
            std::ifstream file(configPath);
            std::smatch match;
            for (std::string line; getline(file, line);)
            {
                if (std::regex_search(line, match, themeMatch))
                {
                    themeMatch = (match.str(1));
                } else if (std::regex_search(line, match, fallbackThemeMatch))
                {
                    fallbackIconTheme = (match.str(1));
                }
                if (iconTheme.isNotEmpty() && fallbackIconTheme.isNotEmpty())
                {
                    break;
                }
            }
            file.close();
        } catch (std::ifstream::failure f)
        {
            DBG("DesktopEntry:failed to read icon theme from .gtkrc-2.0");
        }
    }
    if (iconTheme.isNotEmpty())
    {
        for (int i = 0; i < basePaths.size(); i++)
        {
            checkPaths.push_back(basePaths[i] + iconTheme + "/");
        }
    }
    if (!fallbackIconTheme.isEmpty())
    {
        for (int i = 0; i < basePaths.size(); i++)
        {
            checkPaths.push_back(basePaths[i] + fallbackIconTheme + "/");
        }
    }
    for (int i = 0; i < basePaths.size(); i++)
    {
        checkPaths.push_back(basePaths[i]);
    }

    //run recursive mapping for all subdirectories
    for (int i = 0; i < checkPaths.size(); i++)
    {
        //DBG(String("Mapping icon files under ") + checkPaths[i]);
        findSearchPaths(checkPaths[i]);
    }
    DBG(String("Searching ") + String(searchPaths.size()) + " icon paths:");
    //finally, find and map icon files
    for (const String& path : searchPaths)
    {
        std::vector<String> files = listFiles(path);
        std::regex iconPattern("^(.+)\\.(png|svg|xpm)$", 
                std::regex::ECMAScript | std::regex::icase);
        std::smatch iconMatch;
        for (const String& file : files)
        {
            std::string fileStr = file.toStdString();
            if (std::regex_search(fileStr, iconMatch, iconPattern))
            {
                String filename = iconMatch.str(1);
                if (this->iconPaths[filename].isEmpty())
                {
                    this->iconPaths[filename] = path + iconMatch.str(0);
                }
            }
        }
    }
    DBG(String("Mapped ") + String(iconPaths.size()) + String(" icon files."));
}