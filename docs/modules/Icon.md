# Icon Module Documentation
The Icon module finds and caches icon files. Icon searches follow [freedesktop icon theme specifications](https://specifications.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html). Icon searches run within a dedicated icon thread.

## Public interface
#### [Icon\::Loader](../../Source/Icon/Icon_Loader.h)
Loader objects are used to request icons from the icon thread, or to cancel pending icon requests.

#### [Icon\::Context](../../Source/Icon/Types/Icon_Context.h)
Context lists the types of icon directories found within icon themes.

#### [Icon\::RequestID](../../Source/Icon/Types/Icon_RequestID.h)
RequestID identifies a pending icon request.

## Implementation

#### [Icon\::Cache](../../Source/Icon/Icon_Cache.h)
Cache objects load [GTK icon cache files](https://raw.githubusercontent.com/GNOME/gtk/master/docs/iconcache.txt) to quickly look up icons by name within an icon theme directory.

#### [Icon\::ThemeIndex](../../Source/Icon/Icon_ThemeIndex.h)
ThemeIndex objects read index.theme files within icon theme directories to locate the most appropriate icon file for a request. If available, ThemeIndex objects will use Cache objects to significantly reduce search times.

#### [Icon\::ThreadResource](../../Source/Icon/Icon_ThreadResource.h)
ThreadResource holds and fulfills a queue of icon requests. It uses ThemeIndex objects to locate appropriate icons, and keeps a cache of loaded icon files to decrease thetime needed for future requests.


