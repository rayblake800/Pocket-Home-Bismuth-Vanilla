## LibNM Module Documentation ##

 The LibNM module handles all direct interaction with the libnm-glib library.
Using the GLib module, it provides convenient C++ wrapper objects for all
relevant LibNM objects. These objects automatically manage reference counts 
and other memory management tasks, and provide methods used to call relevant
library functions.

 This module supports any version of libnm-glib from 0.9.10 onwards. Not all
supported libnm-glib versions are threadsafe, so all interaction with
LibNM::Object and its subclasses must occur within a single 
GLib::ThreadResource using the global default GMainContext. The NMThread 
class creates this thread as a SharedResource object, and NMThreadHandler
objects provide threadsafe access to this thread.

 Saved connection data cannot be reliably accessed in older versions of
libnm-glib within multithreaded programs, even if all interaction with libnm
is confined to a single thread. To circumvent this, saved connection data
is read directly from NetworkManager over DBus, using GLib::DBusProxy
subclasses. These classes should be fully threadsafe.

## Public Interface ##

# LibNM/ThreadHandler.h #
  Accesses a shared ThreadResource thread running a GMainLoop with the GLib 
  global default context. All interaction with LibNM::Object subclasses should 
  take place within this thread's event loop.

# LibNM/APData/APHash.h #
  A hash value used to sort, compare, and identify access points, treating 
  access points with identical connection settings as equivalent.

# LibNM/APData/APMode.h #
  Enumerates all Wifi access point types.

# LibNM/APData/SecurityType.h #
  Enumerates all basic Wifi security types.

## NMObjects ##

# LibNM/NMObjects/Object.h #
  The basis for all classes managing LibNM's NMObject* types. This interface 
  provides access to the object's DBus path.

TODO: Finish once Wifi redesigns are finalized.
