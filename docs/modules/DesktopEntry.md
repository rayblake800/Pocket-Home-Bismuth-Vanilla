# DesktopEntry Module Documentation
Desktop entry files are a standardized form of shared application shortcut used on Linux desktops. This standard is provided so that application launchers can provide organized lists of available applications. The DesktopEntry module loads these files into data objects, and provides an interface for reading, updating, and creating desktop entry data. 

The DesktopEntry module exists to help the [AppMenu](./AppMenu.md) module create and edit the application menu. See the full [FreeDesktop Desktop Entry Specifications](https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html) for more information about the desktop entry standard.

## Public Interface

#### [DesktopEntry\::EntryFile](../../Source/Files/DesktopEntry/DesktopEntry_EntryFile.h)
EntryFile objects represent a single .desktop application shortcut file.

#### [DesktopEntry\::Loader](../../Source/Files/DesktopEntry/DesktopEntry_Loader.h)
Loader object provide access to shared desktop entry data.

#### [DesktopEntry\::CallbackID](../../Source/Files/DesktopEntry/Types/DesktopEntry_CallbackID.h)
CallbackId is used to identify or cancel pending desktop entry update callbacks scheduled through the DesktopEntry\::Loader.

#### [DesktopEntry\::UpdateListener](../../Source/Files/DesktopEntry/DesktopEntry_UpdateListener.h)
UpdateListener objects receive notifications when desktop entry data is updated.

#### [DesktopEntry\::FormatError](../../Source/Files/DesktopEntry/Exceptions/DesktopEntry_FormatError.h)
FormatError exceptions signal that an attempt was made to add invalid data to a desktop entry.

#### [DesktopEntry\::FileError](../../Source/Files/DesktopEntry/Exceptions/DesktopEntry_FileError.h)
FileError exceptions signal that an encountered desktop entry file is invalid.

## Private Implementation Classes

#### [DesktopEntry\::LoadingThread](../../Source/Files/DesktopEntry/DesktopEntry_LoadingThread.h)
LoadingThread is the shared thread resource used to load and cache all desktop entry file data.

#### [DesktopEntry\::FileUtils](../../Source/Files/DesktopEntry/DesktopEntry_FileUtils.h)
The FileUtils namespace provides convenience functions for processing desktop entry file data.

#### [DesktopEntry\::UpdateInterface](../../Source/Files/DesktopEntry/DesktopEntry_UpdateInterface.h)
UpdateInterface is the interface used by the LoadingThread to send updates to all UpdateListener objects.
