# GLib Module Documentation
    The GLib module simplifies interaction with GLib-based libraries.

    GLib is a set of low-level C libraries used mainly to implement other libraries. Among other things, it provides a reference-counted object system, an event loop, and tools for inter-process communication over D-Bus. Interacting with GLib is currently only necessary within the Wifi module, but it will eventually be needed for Bluetooth device management and D-Bus application launching.

    The GLib module provides C++ object wrappers for GLib data types. As a C-Based library, GLib requires a lot of manual management of memory and reference counts. The GLib module's object wrappers automatically handle all reference count and memory management through RAII techniques. These wrapper objects are also useful for providing simplified interfaces to that GLib data.

    Besides these basic wrapper classes, the GLib module provides tools to simplify the use of GLib's event loop, object signals, and D-Bus communication features.

#### [GLib::Object](../../Source/GLib/GLib_Object.h)

#### [GLib::WeakRef](../../Source/GLib/GLib_WeakRef.h)

## Borrowed Objects

#### [GLib::Borrowed::Object](../../Source/GLib/Borrowed/GLib_Borrowed_Object.h)

#### [GLib::Borrowed::SharedContainer](../../Source/GLib/Borrowed/GLib_Borrowed_SharedContainer.h)

#### [GLib::Borrowed::ObjectLender](../../Source/GLib/Borrowed/GLib_Borrowed_ObjectLender.h)

## Owned Objects

#### [GLib::Owned::Object](../../Source/GLib/Owned/GLib_Owned_Object.h)

## DBus Proxies

#### [GLib::DBus::Proxy](../../Source/GLib/DBus/GLib_DBus_Proxy.h)

#### [GLib::DBus::SignalHandler](../../Source/GLib/DBus/GLib_DBus_SignalHandler.h)

### GVariant Handling

#### [GLib::BasicValueWrapper](../../Source/GLib/DBus/Variant/GLib_BasicValueWrapper.h)

#### [GLib::VariantTypes](../../Source/GLib/DBus/Variant/GLib_VariantTypes.h)

#### [GLib::VariantConverter](../../Source/GLib/DBus/Variant/GLib_VariantConverter.h)

#### [GLib::Variant](../../Source/GLib/DBus/Variant/GLib_Variant.h)

#### [GLib::TypeHelper](../../Source/GLib/DBus/Variant/GLib_TypeHelper.h)

## Signal Handling

#### [GLib::Signal::Handler](../../Source/GLib/Signal/GLib_Signal_Handler.h)

#### [GLib::Signal::Connection](../../Source/GLib/Signal/GLib_Signal_Connection.h)

#### [GLib::Signal::CallbackData](../../Source/GLib/Signal/GLib_Signal_CallbackData.h)

## Smart Pointers

#### [GLib::ScopedGPointer](../../Source/GLib/SmartPointers/GLib_ScopedGPointer.h)

#### [GLib::VariantPtr](../../Source/GLib/SmartPointers/GLib_VariantPtr.h)

#### [GLib::ErrorPtr](../../Source/GLib/SmartPointers/GLib_ErrorPtr.h)

#### [GLib::SharedContextPtr](../../Source/GLib/SmartPointers/GLib_SharedContextPtr.h)

#### [GLib::ContextPtr](../../Source/GLib/SmartPointers/GLib_ContextPtr.h)

#### [GLib::LoopPtr](../../Source/GLib/SmartPointers/GLib_LoopPtr.h)

## GLib Event Threads

#### [GLib::EventLoop](../../Source/GLib/Thread/GLib_EventLoop.h)

#### [GLib::ContextCaller](../../Source/GLib/Thread/GLib_ContextCaller.h)

#### [GLib::ThreadHandler](../../Source/GLib/Thread/GLib_ThreadHandler.h)

#### [GLib::SharedThread](../../Source/GLib/Thread/GLib_SharedThread.h)

#### [GLib::ObjectPtr](../../Source/GLib/SmartPointers/GLib_ObjectPtr.h)

