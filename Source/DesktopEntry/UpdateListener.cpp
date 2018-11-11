#define DESKTOP_ENTRY_LOADER_IMPLEMENTATION
#include "DesktopEntry/UpdateListener.h"
#include "DesktopEntry/LoadingThread.h"

DesktopEntry::UpdateListener::UpdateListener() : 
    ResourceHandler<LoadingThread>(LoadingThread::resourceKey,
            []()->SharedResource* { return new LoadingThread(); }) { }
