#pragma once

namespace Wifi { class ConnectionUpdateInterface; }
namespace Wifi { class AccessPoint; }

class Wifi::ConnectionUpdateInterface
{
public:
    ConnectionUpdateInterface() { }

    virtual ~ConnectionUpdateInterface() { }

private:
    virtual void startedConnecting(const AccessPoint connectingAP) = 0;

    virtual void connectionAuthFailed(const AccessPoint connectingAP) = 0;

    virtual void connected(const AccessPoint connectedAP) = 0; 

    virtual void disconnected(const AccessPoint disconnectedAP) = 0; 
};
