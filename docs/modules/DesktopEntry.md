# DesktopEntry Module Documentation
Desktop entry files are a standardized form of shared application shortcut used on Linux desktops. This standard is provided so that application launchers can provide organized lists of available applications. The DesktopEntry module loads these files into data objects, and provides an interface for reading, updating, and changing desktop entry data. 

See https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html for the full desktop entry specification.

## Public Interface

#### [DesktopEntry::EntryFile](../../Source/DesktopEntry/DesktopEntry_EntryFile.h)
  Represents a single .desktop application shortcut file.

#### [DesktopEntry::Loader](../../Source/DesktopEntry/DesktopEntry_Loader.h)
  Provides an interface for accessing shared desktop entry data.

#### [DesktopEntry::CallbackID](../../Source/DesktopEntry/Types/DesktopEntry_CallbackID.h)
  Used to identify or cancel pending desktop entry update callbacks scheduled through the DesktopEntry::Loader.

#### [DesktopEntry::UpdateListener](../../Source/DesktopEntry/DesktopEntry_UpdateListener.h)
  Receives notifications when desktop entry data is updated.

#### [DesktopEntry::FormatError](../../Source/DesktopEntry/Exceptions/DesktopEntry_FormatError.h)
  Signals that an attempt was made to add invalid data to a desktop entry.

#### [DesktopEntry::FileError](../../Source/DesktopEntry/Exceptions/DesktopEntry_FileError.h)
  Signals that an encountered desktop entry file is invalid.

## Private Implementation Classes

#### [DesktopEntry::LoadingThread](../../Source/DesktopEntry/DesktopEntry_LoadingThread.h)
  The shared thread resource used to load and cache all desktop entry file data.

#### [DesktopEntry::FileUtils](../../Source/DesktopEntry/DesktopEntry_FileUtils.h)
  Provides convenience functions for processing desktop entry file data.

#### [DesktopEntry::UpdateInterface](../../Source/DesktopEntry/DesktopEntry_UpdateInterface.h)
  The interface used by the LoadingThread to send updates to all UpdateListener objects.


