#define DBUS_THREAD_IMPLEMENTATION
#include "GLib_DBusThreadRunner.h"
#include "GLib_DBusThread.h"

GLib::DBusThreadRunner::DBusThreadRunner()
{
    SharedResource::LockedPtr<DBusThread> dbusThread = getWriteLockedResource();
    dbusThread->startThreadResource();
}

GLib::DBusThreadRunner::~DBusThreadRunner()
{
    SharedResource::LockedPtr<DBusThread> dbusThread = getWriteLockedResource();
    dbusThread->stopThreadAndWait();
}
