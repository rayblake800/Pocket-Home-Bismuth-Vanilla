#pragma once
#include "GLib/DBus/DBusProxy.h"
#include "GLib/ThreadResource.h"

class GLib::DBusThread : public ThreadResource
{
public:
    DBusThread();
    virtual ~DBusThread();
};
