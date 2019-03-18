# GLib Module Documentation
The GLib module simplifies interaction with GLib-based libraries needed to implement other modules.

GLib is a set of low-level C libraries used mainly to implement other libraries. Among other things, it provides a reference-counted object system, an event loop, and tools for inter-process communication over D-Bus. Interacting with GLib is currently only necessary within the [Wifi](./Wifi.md) module, but it will eventually be needed for [Bluetooth](./Bluetooth.md) device management and D-Bus application launching.

The GLib module provides C++ object wrappers for GLib data types. As a C-Based library, GLib requires a lot of manual management of memory and reference counts. The GLib module's object wrappers automatically handle all reference count and memory management through RAII techniques. These wrapper objects are also useful for providing simplified interfaces to that GLib data.

Besides these basic wrapper classes, the GLib module provides tools to simplify the use of GLib's event loop, object signals, and D-Bus communication features.

## GObject Management
The main purpose of the GLib module is to provide C++ object wrappers for GObject data pointers, GLib's custom C-based object system.

### Owned Objects
The GLib library uses a reference counting system to control when objects are destroyed. At any time, a GObject may be referenced, and it will only be destroyed once all of those references are removed. When the GLib module refers to a GObject wrapper class as being owned, it means that objects of that class will claim a reference to GObject data it represents, preventing it from being destroyed while the wrapper object exists.

#### [GLib\::Owned\::Object](../../Source/Framework/GLib/Owned/GLib_Owned_Object.h)
Owned\::Object is an abstract basis for owned GObject wrapper classes. Wrapper classes for most GObject types should be written as GLib\::Owned\::Object subclasses.

### Borrowed Objects
Although a GObject won't be destroyed automatically while it still has remaining references, there's nothing to prevent it from being manually destoyed. When the GLib module refers to a GObject wrapper class as borrowed, it means that its GObject data may be destroyed at any time by the library that provides it, regardless of reference count.  GLib\::WeakRef cannot be safely used with borrowed object data.

#### [GLib\::Borrowed\::Object](../../Source/Framework/GLib/Borrowed/GLib_Borrowed_Object.h)
Borrowed\::Object is an abstract basis for borrowed GObject wrapper classes. Wrapper classes should be written as GLib\::Borrowed\::Object subclasses if their object data may be suddenly destroyed by the library that provides it.

#### [GLib\::Borrowed\::ObjectLender](../../Source/Framework/GLib/Borrowed/GLib_Borrowed_ObjectLender.h)
Each ObjectLender object controls access to a set of borrowed object instances. Borrowed\::Object instances should only be directly created from GObject data by an ObjectLender. When borrowed GObject data becomes invalid, the ObjectLender should be notified so it can mark every single Object wrapper that holds that data as invalid.

### Internal GObject Implementation Classes

#### [GLib\::Object](../../Source/Framework/GLib/GLib_Object.h)
GLib\::Object is the abstract base class for all GLib GObject wrapper classes. It is responsible for GObject type checking, object comparison, and object property access. Wrapper classes should not directly inherit from GLib\::Object. Instead, they should use GLib\::Owned\::Object or GLib\::Borrowed\::Object, depending on the properties of the managed GObject type.

#### [GLib\::WeakRef](../../Source/Framework/GLib/GLib_WeakRef.h)
GLib\::WeakRef objects manage GLib's GWeakRef weak reference data structures. These structures are used to provide safe access to GObject data, without preventing that data from being destroyed. This is primarily used internally by GLib\::Owned\::Object classes.

#### [GLib\::Borrowed\::SharedContainer](../../Source/Framework/GLib/Borrowed/GLib_Borrowed_SharedContainer.h)
SharedContainer objects are internal data containers used by Borrowed\::Object instances to share their GObject data. Each borrowed GObject pointer is only held in a single SharedContainer, so the ObjectLender can ensure every Borrowed\::Object holding that data is invalidated simultaneously when the GObject pointer becomes invalid.

## Signal Handling
All GObjects may create and send class-specific signals to registered signal handlers. The GLib module provides classes to simplify the process of creating signal handlers and handling signals.

#### [GLib\::Signal\::Handler](../../Source/Framework/GLib/Signal/GLib_Signal_Handler.h)
Signal\::Handler is an abstract basis for classes that handle GLib signals from a single GObject type. Handler subclasses handle the process of connecting to all relevant signals emitted by their GObject signal sources, and provide virtual methods to handle received signals.

### Signal Handler Implementation Classes
#### [GLib\::Signal\::Connection](../../Source/Framework/GLib/Signal/GLib_Signal_Connection.h)
Each Connection object represents a single connection between a signal handler and a GObject signal source. Creating the object establishes the connection, and the resulting connection object may then be used to access the signal source or close the connection.

#### [GLib\::Signal\::CallbackData](../../Source/Framework/GLib/Signal/GLib_Signal_CallbackData.h)
CallbackData objects store whatever data is needed to properly pass a GObject signal from a static callback function back to a signal handler object.

## GLib Event Handling
GLib libraries handle signal transmission and asynchronous actions using an event loop running on a dedicated thread. The GLib module provides tools for creating an event loop on its own thread, and for making calls within that thread.

#### [GLib\::EventLoop](../../Source/Framework/GLib/Thread/GLib_EventLoop.h)
EventLoop creates, runs, and destroys a GLib event loop, and provides access to that loop through its GLib context object.

#### [GLib\::ContextCaller](../../Source/Framework/GLib/Thread/GLib_ContextCaller.h)
ContextCaller objects use a GLib context to call functions within a GLib event loop that shares its context. ContextCaller can block the calling thread until the requested function call is complete, or schedule them to run asynchronously.

#### [GLib\::SharedThread](../../Source/Framework/GLib/Thread/GLib_SharedThread.h)
SharedThread is an abstract juce\::Thread class that runs an EventLoop and provides access to the loop through a ContextCaller. SharedThread is meant to be implemented as a [shared resource](./SharedResource.md) class, either inheriting from [SharedResource\::Resource](../../Source/Util/SharedResource/SharedResource_Resource.h), or [SharedResource\::Modular\::Module](../../Source/Util/SharedResource/Modular/SharedResource_Modular_Module.h).

#### [GLib\::ThreadHandler](../../Source/Framework/GLib/Thread/GLib_ThreadHandler.h)
ThreadHandler is an abstract basis for a class that manages a GLib\::SharedThread subclass instance. Every object of a given ThreadHandler subclass safely shares access to a single SharedThread subclass instance.

## D-Bus Communication
D-Bus is a system providing inter-process communication and remote method calling in GNU/Linux. The GLib library provides an interface programs may use to communicate through D-Bus. The GLib module provides tools for using and managing this interface.

#### [GLib\::DBus\::Proxy](../../Source/Framework/GLib/DBus/GLib_DBus_Proxy.h)
Proxy is an Owned\::Object subclass that represents a remote object within another process, accessed over D-Bus. It is able to read and change the remote object's properties, and call methods that the remote object provides. Each unique D-Bus object type should be accessed through its own DBus\::Proxy subclass.

#### [GLib\::DBus\::SignalHandler](../../Source/Framework/GLib/DBus/GLib_DBus_SignalHandler.h)
DBus\::SignalHandler is a subclass of GLib\::Signal\::Handler that handles signals emitted over D-Bus through a DBus\::Proxy object. Each Proxy subclass that requires signal handling should provide a SignalHandler subclass to handle the specific signals that object will create.

### GVariant Handling
GLib transfers all data over D-Bus packaged in GVariant data structures. GVariant structures may store any number of data values, either individually, or packaged in arrays, tuples, or dictionaries. GVariant structures are immutable and reference counted. The GLib module provides tools for extracting data from GVariant structures, and for packaging data into GVariant structures.

#### [GLib\::VariantConverter](../../Source/Framework/GLib/DBus/Variant/GLib_VariantConverter.h)
GLib\::VariantConverter provides functions that extract data from basic GVariants, iterate over GVariant containers or dictionaries, or package data into new GVariant structures.

#### [GLib\::VariantTypes](../../Source/Framework/GLib/DBus/Variant/GLib_VariantTypes.h)
VariantTypes enumerates every possible data type a GVariant may hold.

#### [GLib\::Variant](../../Source/Framework/GLib/DBus/Variant/GLib_Variant.h)
Variant is an incomplete attempt at making a comprehensive wrapper class for holding and accessing any type of GVariant data. Once fully designed, implemented, and tested, GLib\::Variant will completely replace GLib\::VariantConverter.

#### [GLib\::TypeHelper](../../Source/Framework/GLib/DBus/Variant/GLib_TypeHelper.h)
TypeHelper is an incomplete internal data class to be used by GLib\::Variant. TypeHelper provides data access functions for every data type a GVariant may hold.

#### [GLib\::BasicValueWrapper](../../Source/Framework/GLib/DBus/Variant/GLib_BasicValueWrapper.h)
Some data types held by GVariant structures are considered distinct and treated differently by the GVariant even though their equivalent types in C++ are identical. BasicValueWrapper is a template class used to represent these types, so that they can each have distinct template function specializations in GLib\::TypeHelper.

## Smart Pointers
GLib library functions often return values that need to be freed or dereferenced once they are no longer needed. The GLib module provides several pointer classes that will automatically handle this process on destruction.

#### [GLib\::ScopedGPointer](../../Source/Framework/GLib/SmartPointers/GLib_ScopedGPointer.h)
ScopedGPointer is a template class used to construct other smart pointer classes. Its template parameters define the type of GLib data it holds, and the function it calls to dereference or free that data. Avoid using ScopedGPointer objects with data that needs to be copied or shared.

#### [GLib\::VariantPtr](../../Source/Framework/GLib/SmartPointers/GLib_VariantPtr.h)
VariantPtr holds a GVariant data value. If its held value is non-null, it will dereference its held GVariant pointer on destruction.

#### [GLib\::ErrorPtr](../../Source/Framework/GLib/SmartPointers/GLib_ErrorPtr.h)
ErrorPtr may be used with GLib error handling in place of GError data pointers. If a GLib function assigns error data to an ErrorPtr object, it can handle that error, either with a custom error handling function, or by printing the error's message if no custom function is provided. ErrorPtr will free the GError data assigned to it immediately after handling the error. Errors are handled either when the ErrorPtr is destroyed, or whenever the ErrorPtr object's handleError method is called.

#### [GLib\::SharedContextPtr](../../Source/Framework/GLib/SmartPointers/GLib_SharedContextPtr.h)
SharedContextPtr manages a GMainContext data pointer. SharedContextPtr automatically increases its GMainContext's reference count when copied, and decreases it on destruction.

#### [GLib\::ContextPtr](../../Source/Framework/GLib/SmartPointers/GLib_ContextPtr.h)
ContextPtr is a single use version of SharedContextPtr that cannot be directly copied.

#### [GLib\::LoopPtr](../../Source/Framework/GLib/SmartPointers/GLib_LoopPtr.h)
LoopPtr is a ScopedGPointer that holds a GLib event loop pointer, dereferencing it on destruction.

#### [GLib\::ObjectPtr](../../Source/Framework/GLib/SmartPointers/GLib_ObjectPtr.h)
ObjectPtr creates a temporary managed pointer to a GObject* held by a GLib\::Object. It automatically determines whether it needs to dereference its data on destruction based on the object used to create it.

