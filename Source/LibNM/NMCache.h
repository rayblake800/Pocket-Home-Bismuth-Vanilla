#pragma once
#include "GLib/ThreadHandler.h"
#include "LibNM/NMThread.h"
#include "LibNM/NMObjects/AccessPoint.h"

class LibNM::NMCache : private GLib::ThreadHandler
{
public:
    NMCache();

    virtual ~NMCache();


};
