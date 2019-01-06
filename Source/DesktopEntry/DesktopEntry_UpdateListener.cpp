#define DESKTOP_ENTRY_IMPLEMENTATION
#include "DesktopEntry_UpdateListener.h"
#include "DesktopEntry_LoadingThread.h"

DesktopEntry::UpdateListener::UpdateListener() : 
    SharedResource::Handler<LoadingThread>() { }
