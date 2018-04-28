#pragma once
#include <nm-client.h>
#include "NMPPConnection.h"
#include "NMPPDeviceWifi.h"
#include "GPPObject.h"

class NMPPClient : public GPPObject
{
public:
    NMPPClient();
    
    NMPPClient(const NMPPClient& toCopy);
    
    Array<NMPPDeviceWifi> getWifiDevices();
    
    NMPPDeviceWifi getWifiDeviceByIface(const char* interface);
    
    NMPPDeviceWifi getWifiDeviceByPath(const char* path);
    
    Array<NMPPConnection> getActiveConnections();
    
    NMPPConnection getPrimaryConnection();
    
    NMPPConnection getActivatingConnection();
    
    void deactivateConnection(const NMPPConnection& activeCon);
    
    bool wifiEnabled();
    
    void setWifiEnabled(bool enabled);
    
    class ConnectionHandler
    {
    public:
        ConnectionHandler();
        
        virtual ConnectionHandler();
        
        void activateConnection(
                const NMPPClient& client,
                const NMPPConnection& connection,
                const NMPPAccessPoint& accessPoint);
        
        void addAndActivateConnection(
                const NMPPClient& client,
                const NMPPConnection& connection,
                const NMPPDeviceWifi& wifiDevice,
                const NMPPAccessPoint& accessPoint);
    private:
        virtual void openingConnection(NMPPClient* client,
                NMPPConnection connection, bool isNew);
        
        virtual void openingConnectionFailed(NMPPClient* client,
                GError* error, bool isNew);
    };
    
    class Listener : public GPPObject::SignalHandler
    {
    public:
        Listener() { }
        
        virtual ~Listener() { }
        
    private:
        virtual void wirelessStateChange(bool wifiEnabled);
        
        void propertyChanged(GPPObject* source, String property) override;  
    };
    
    void addListener(Listener* listener);
    
    void removeListener(Listener* listener);

private:
    GType getType() const override;
};