#pragma once
#include "Wifi/Connection/ConnectionUpdateInterface.h"
#include "SharedResource/Handler.h"

namespace Wifi { class ConnectionListener; }
namespace Wifi { class ConnectionTracker; }

class Wifi::ConnectionListener : public ConnectionUpdateInterface,
    public SharedResource::Handler<ConnectionTracker>
{
public:
    ConnectionListener() { };

    virtual ~ConnectionListener() { }

private:
    virtual void startedConnecting(const AccessPoint connectingAP) override;

    virtual void connectionAuthFailed(const AccessPoint connectingAP) override;

    virtual void connected(const AccessPoint connectedAP) override; 

    virtual void disconnected(const AccessPoint disconnectedAP) override; 
};
