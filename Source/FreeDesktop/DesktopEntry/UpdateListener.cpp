#define DESKTOP_ENTRY_IMPLEMENTATION_ONLY
#include "DesktopEntry/UpdateListener.h"
#include "DesktopEntry/LoadingThread.h"

DesktopEntry::UpdateListener::UpdateListener() : 
    SharedResource::Handler<LoadingThread>() { }
