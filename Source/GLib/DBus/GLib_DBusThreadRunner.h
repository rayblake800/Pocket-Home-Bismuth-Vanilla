#pragma once
/**
 * @file  GLib_DBusThreadRunner.h
 *
 * @brief  Starts the DBus thread on construction, and stops it on destruction.
 */

#include "SharedResource_Handler.h"

namespace GLib { class DBusThread; }
namespace GLib { class DBusThreadRunner; }

class GLib::DBusThreadRunner : public SharedResource::Handler<DBusThread>
{
public:
    DBusThreadRunner();
    virtual ~DBusThreadRunner();
};
