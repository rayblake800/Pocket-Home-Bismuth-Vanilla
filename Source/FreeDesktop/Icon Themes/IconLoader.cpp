#include "IconLoader.h"

IconLoader::IconLoader :
ResourceHandler<IconThread>(IconThread::resourceKey,
        []()->SharedResource* { return new IconThread(); }) { }
