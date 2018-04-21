#pragma once
#include "GDBusProxyInterface.h"
#include "SavedConnection.h"

class SavedConnections : public GDBusProxyInterface
{
public:
    SavedConnections();
    
    virtual ~SavedConnections() { }
    
    Array<SavedConnection> getConnections();
    
    
    
private:
    Array<SavedConnection> connections;
};