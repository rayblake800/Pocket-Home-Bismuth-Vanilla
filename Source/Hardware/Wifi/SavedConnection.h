#pragma once
#include "GDBusProxyInterface.h"

class SavedConnection : public GDBusProxyInterface
{
public:
    SavedConnection(const char* path);
    
    virtual ~SavedConnection() { }
};