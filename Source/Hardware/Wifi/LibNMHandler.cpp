
#include <nm-remote-connection.h>
#include <nm-device.h>
#include <glib-2.0/glib/gerror.h>
#include <glib-2.0/glib/garray.h>
#include "MainConfigFile.h"
#include "WifiAccessPoint.h"
#include "LibNMHandler.h"
#include <libnm-glib/nm-remote-settings.h>
#include <nm-connection.h>
#include <glib-2.0/glib/gslist.h>


static const char* nmStateStr(NMState state)
{
    switch(state)
    {
        case NM_STATE_UNKNOWN:
            return "NM_STATE_UNKNOWN";
        case NM_STATE_ASLEEP:
            return "NM_STATE_ASLEEP";
        case NM_STATE_DISCONNECTED:
            return "NM_STATE_DISCONNECTED";
        case NM_STATE_DISCONNECTING:
            return "NM_STATE_DISCONNECTING";
        case NM_STATE_CONNECTING:
            return "NM_STATE_CONNECTING";
        case NM_STATE_CONNECTED_LOCAL:
            return "NM_STATE_CONNECTED_LOCAL";
        case NM_STATE_CONNECTED_SITE:
            return "NM_STATE_CONNECTED_SITE";
        case NM_STATE_CONNECTED_GLOBAL:
            return "NM_STATE_CONNECTED_GLOBAL";
    }
}

static const char* nmConnectivityStateStr(NMConnectivityState state)
{
    switch(state)
    {
        case NM_CONNECTIVITY_UNKNOWN:
            return "NM_CONNECTIVITY_UNKNOWN";
        case NM_CONNECTIVITY_NONE:
            return "NM_CONNECTIVITY_NONE";
        case NM_CONNECTIVITY_PORTAL:
            return "NM_CONNECTIVITY_PORTAL";
        case NM_CONNECTIVITY_LIMITED:
            return "NM_CONNECTIVITY_LIMITED";
        case NM_CONNECTIVITY_FULL:
            return "NM_CONNECTIVITY_FULL";
    }
}

static void nmClientPrint(NMClient* nmClient)
{
    std::cout << "\tClient version : " << nm_client_get_version(nmClient) << "\n"
              << "\tClient state   : " << nmStateStr(nm_client_get_state(nmClient)) << "\n"
              << "\tStartup        : " << (nm_client_get_startup(nmClient) ? "yes\n" : "no\n")
              << "\tDaemon running : " << (nm_client_get_manager_running(nmClient) ? "yes\n" : "no\n")
              << "\tConnectivity   : " << nmConnectivityStateStr(nm_client_get_connectivity(nmClient)) << "\n";
    
    std::cout << "\n\tNetworking enabled : " 
              <<  (nm_client_networking_get_enabled(nmClient) ? "yes\n" : "no\n");

              
    std::cout << "\tWireless enabled   : " 
              <<  (nm_client_wireless_get_enabled(nmClient) ? "yes " : "no  ")
              << "\thardware support: " 
              <<  (nm_client_wireless_hardware_get_enabled(nmClient) ? "yes\n" : "no\n");  
    
    std::cout << "\tWWAN enabled       : " 
              <<  (nm_client_wwan_get_enabled(nmClient) ? "yes " : "no  ")
              << "\thardware support: " 
              <<  (nm_client_wwan_hardware_get_enabled(nmClient) ? "yes\n" : "no\n"); 


    std::cout << "\tWIMAX enabled      : " 
              <<  (nm_client_wimax_get_enabled(nmClient) ? "yes " : "no  ")
              << "\thardware support: " 
              <<  (nm_client_wimax_hardware_get_enabled(nmClient) ? "yes\n" : "no\n");  
              
    std::cout << "\n\tClient logging info:\n";
    
    //check logging
    char* level = nullptr;
    char* domains = nullptr;
    GError* error = nullptr;
    if(nm_client_get_logging(nmClient, &level, &domains, &error))
    {
        std::cout << "\t\tLogging level: " << level << "\n";
        std::cout << "\t\tLogging domains: " << domains << "\n";
    }
    else
    {
        std::cout << "\t\tFailed to get client logging info.\n";
        if(error != nullptr)
        {
            std::cout << "\t\tError:" << error->message << "\n";
            g_error_free(error);
            error = nullptr;
        }
    }
}

static void nmDevicePrint(NMDevice* nmDevice)
{
    const char* type       = nm_device_get_type_description(nmDevice);
    const char* descriptn  = nm_device_get_description(nmDevice);
    const char* iface      = nm_device_get_iface(nmDevice);
    const char* setType    = g_type_name(nm_device_get_setting_type(nmDevice));
    const char* ip_iface   = nm_device_get_ip_iface(nmDevice);
    const char* udi        = nm_device_get_udi(nmDevice);
    const char* driver     = nm_device_get_driver(nmDevice);
    const char* driver_vsn = nm_device_get_driver_version(nmDevice);
    const char* fw_vsn     = nm_device_get_firmware_version(nmDevice);
    const char* mac_addr   = nm_device_get_hw_address(nmDevice);
    const char* port_id    = nm_device_get_physical_port_id(nmDevice);
    const char* product    = nm_device_get_product(nmDevice);
    const char* vendor     = nm_device_get_vendor(nmDevice);
    
    bool fw_missing        =  nm_device_get_firmware_missing(nmDevice);
    bool autoconnect       =  nm_device_get_firmware_missing(nmDevice);
    
    
    guint32 mtu = nm_device_get_mtu(nmDevice);
    
    NMDeviceCapabilities capabilities = nm_device_get_capabilities(nmDevice);
    NMDeviceStateReason stateReason;
    NMDeviceState device_state = nm_device_get_state_reason(nmDevice, &stateReason);
    
    std::cout << "\tType             : " << (type       ? type       : "unknown") << "\n"
              << "\tDescription      : " << (descriptn  ? descriptn  : "unknown") << "\n"
              << "\tInterface        : " << (iface      ? iface      : "unknown") << "\n"
              << "\tSetting Type     : " << (setType    ? setType    : "unknown") << "\n"
              << "\tIP Interface     : " << (ip_iface   ? ip_iface   : "unknown") << "\n"
              << "\tUDI              : " << (udi        ? udi        : "unknown") << "\n"
              << "\tDriver           : " << (driver     ? driver     : "unknown") << "\n"
              << "\tDriver Version   : " << (driver_vsn ? driver_vsn : "unknown") << "\n"
              << "\tFirmware Version : " << (fw_vsn     ? fw_vsn     : "unknown") << "\n"
              << "\tHardware Address : " << (mac_addr   ? mac_addr   : "unknown") << "\n"
              << "\tPort ID          : " << (port_id    ? port_id    : "unknown") << "\n"
              << "\tProduct          : " << (product    ? product    : "unknown") << "\n"
              << "\tVendor           : " << (vendor     ? vendor     : "unknown") << "\n";
              
    std::cout << "\tMTU              : " << mtu << "\n";
              
    std::cout << "\tFirmware found   : " << (fw_missing  ? "no"  : "yes" ) << "\n"
              << "\tAutoconnect      : " << (autoconnect ? "yes" : "no"  ) << "\n\n";
              
    std::cout << "\tDevice Capabilities : (" << (int) capabilities << ")\n";
    if(capabilities == NM_DEVICE_CAP_NONE)
    {
        std::cout << "\t\tnone\n";
    }
    if((capabilities & NM_DEVICE_CAP_NM_SUPPORTED) == NM_DEVICE_CAP_NM_SUPPORTED)
    {
        std::cout << "\t\tDevice is NetworkManager Supported\n";
    }
    if((capabilities & NM_DEVICE_CAP_CARRIER_DETECT) == NM_DEVICE_CAP_CARRIER_DETECT)
    {
        std::cout << "\t\tDevice can indicate carrier status\n";
    }
    /*
    if((capabilities & NM_DEVICE_CAP_IS_SOFTWARE) == NM_DEVICE_CAP_IS_SOFTWARE)
    {
        std::cout << "\t\tDevice is software\n";
    }
    */
    
    std::cout << "\tDevice State        : ";
    switch(device_state)
    {
        case NM_DEVICE_STATE_UNKNOWN:
            std::cout << "NM_DEVICE_STATE_UNKNOWN\n";
            break;
        case NM_DEVICE_STATE_UNMANAGED:
            std::cout << "NM_DEVICE_STATE_UNMANAGED\n";
            break;
        case NM_DEVICE_STATE_UNAVAILABLE:
            std::cout << "NM_DEVICE_STATE_UNAVAILABLE\n";
            break;
        case NM_DEVICE_STATE_DISCONNECTED:
            std::cout << "NM_DEVICE_STATE_DISCONNECTED\n";
            break;
        case NM_DEVICE_STATE_PREPARE:
            std::cout << "NM_DEVICE_STATE_PREPARE\n";
            break;
        case NM_DEVICE_STATE_CONFIG:
            std::cout << "NM_DEVICE_STATE_CONFIG\n";
            break;
        case NM_DEVICE_STATE_NEED_AUTH:
            std::cout << "NM_DEVICE_STATE_NEED_AUTH\n";
            break;
        case NM_DEVICE_STATE_IP_CONFIG:
            std::cout << "NM_DEVICE_STATE_IP_CONFIG\n";
            break;
        case NM_DEVICE_STATE_SECONDARIES:
            std::cout << "NM_DEVICE_STATE_SECONDARIES\n";
            break;
        case NM_DEVICE_STATE_ACTIVATED:
            std::cout << "NM_DEVICE_STATE_ACTIVATED\n";
            break;
        case NM_DEVICE_STATE_DEACTIVATING:
            std::cout << "NM_DEVICE_STATE_DEACTIVATING\n";
            break;
        case NM_DEVICE_STATE_FAILED:
            std::cout << "NM_DEVICE_STATE_FAILED\n";
            break;
    }
    
    std::cout << "\tDevice State Reason : ";
    switch(stateReason)
    {
        case NM_DEVICE_STATE_REASON_NONE:
            std::cout << "NM_DEVICE_STATE_REASON_NONE\n";
            break;
        case NM_DEVICE_STATE_REASON_UNKNOWN:
            std::cout << "NM_DEVICE_STATE_REASON_UNKNOWN\n";
            break;
        case NM_DEVICE_STATE_REASON_NOW_MANAGED:
            std::cout << "NM_DEVICE_STATE_REASON_NOW_MANAGED\n";
            break;
        case NM_DEVICE_STATE_REASON_NOW_UNMANAGED:
            std::cout << "NM_DEVICE_STATE_REASON_NOW_UNMANAGED\n";
            break;
        case NM_DEVICE_STATE_REASON_CONFIG_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_CONFIG_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE:
            std::cout << "NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE\n";
            break;
        case NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED:
            std::cout << "NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED\n";
            break;
        case NM_DEVICE_STATE_REASON_NO_SECRETS:
            std::cout << "NM_DEVICE_STATE_REASON_NO_SECRETS\n";
            break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_DISCONNECT:
            std::cout << "NM_DEVICE_STATE_REASON_SUPPLICANT_DISCONNECT\n";
            break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_CONFIG_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_SUPPLICANT_CONFIG_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_SUPPLICANT_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_TIMEOUT:
            std::cout << "NM_DEVICE_STATE_REASON_SUPPLICANT_TIMEOUT\n";
            break;
        case NM_DEVICE_STATE_REASON_PPP_START_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_PPP_START_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_PPP_DISCONNECT:
            std::cout << "NM_DEVICE_STATE_REASON_PPP_DISCONNECT\n";
            break;
        case NM_DEVICE_STATE_REASON_PPP_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_PPP_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_DHCP_START_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_DHCP_START_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_DHCP_ERROR:
            std::cout << "NM_DEVICE_STATE_REASON_DHCP_ERROR\n";
            break;
        case NM_DEVICE_STATE_REASON_DHCP_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_DHCP_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_SHARED_START_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_SHARED_START_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_SHARED_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_SHARED_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_AUTOIP_START_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_AUTOIP_START_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_AUTOIP_ERROR:
            std::cout << "NM_DEVICE_STATE_REASON_AUTOIP_ERROR\n";
            break;
        case NM_DEVICE_STATE_REASON_AUTOIP_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_AUTOIP_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_BUSY:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_BUSY\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_NO_DIAL_TONE:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_NO_DIAL_TONE\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_NO_CARRIER:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_NO_CARRIER\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_DIAL_TIMEOUT:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_DIAL_TIMEOUT\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_DIAL_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_DIAL_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_INIT_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_INIT_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_APN_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_APN_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_NOT_SEARCHING:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_REGISTRATION_NOT_SEARCHING\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_DENIED:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_REGISTRATION_DENIED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_TIMEOUT:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_REGISTRATION_TIMEOUT\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_REGISTRATION_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_REGISTRATION_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_PIN_CHECK_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_PIN_CHECK_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_FIRMWARE_MISSING:
            std::cout << "NM_DEVICE_STATE_REASON_FIRMWARE_MISSING\n";
            break;
        case NM_DEVICE_STATE_REASON_REMOVED:
            std::cout << "NM_DEVICE_STATE_REASON_REMOVED\n";
            break;
        case NM_DEVICE_STATE_REASON_SLEEPING:
            std::cout << "NM_DEVICE_STATE_REASON_SLEEPING\n";
            break;
        case NM_DEVICE_STATE_REASON_CONNECTION_REMOVED:
            std::cout << "NM_DEVICE_STATE_REASON_CONNECTION_REMOVED\n";
            break;
        case NM_DEVICE_STATE_REASON_USER_REQUESTED:
            std::cout << "NM_DEVICE_STATE_REASON_USER_REQUESTED\n";
            break;
        case NM_DEVICE_STATE_REASON_CARRIER:
            std::cout << "NM_DEVICE_STATE_REASON_CARRIER\n";
            break;
        case NM_DEVICE_STATE_REASON_CONNECTION_ASSUMED:
            std::cout << "NM_DEVICE_STATE_REASON_CONNECTION_ASSUMED\n";
            break;
        case NM_DEVICE_STATE_REASON_SUPPLICANT_AVAILABLE:
            std::cout << "NM_DEVICE_STATE_REASON_SUPPLICANT_AVAILABLE\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_NOT_FOUND:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_NOT_FOUND\n";
            break;
        case NM_DEVICE_STATE_REASON_BT_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_BT_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_NOT_INSERTED:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_SIM_NOT_INSERTED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_PIN_REQUIRED:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_SIM_PIN_REQUIRED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_PUK_REQUIRED:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_SIM_PUK_REQUIRED\n";
            break;
        case NM_DEVICE_STATE_REASON_GSM_SIM_WRONG:
            std::cout << "NM_DEVICE_STATE_REASON_GSM_SIM_WRONG\n";
            break;
        case NM_DEVICE_STATE_REASON_INFINIBAND_MODE:
            std::cout << "NM_DEVICE_STATE_REASON_INFINIBAND_MODE\n";
            break;
        case NM_DEVICE_STATE_REASON_DEPENDENCY_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_DEPENDENCY_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_BR2684_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_BR2684_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_MANAGER_UNAVAILABLE:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_MANAGER_UNAVAILABLE\n";
            break;
        case NM_DEVICE_STATE_REASON_SSID_NOT_FOUND:
            std::cout << "NM_DEVICE_STATE_REASON_SSID_NOT_FOUND\n";
            break;
        case NM_DEVICE_STATE_REASON_SECONDARY_CONNECTION_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_SECONDARY_CONNECTION_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_DCB_FCOE_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_DCB_FCOE_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_TEAMD_CONTROL_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_TEAMD_CONTROL_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_FAILED:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_FAILED\n";
            break;
        case NM_DEVICE_STATE_REASON_MODEM_AVAILABLE:
            std::cout << "NM_DEVICE_STATE_REASON_MODEM_AVAILABLE\n";
            break;
        case NM_DEVICE_STATE_REASON_SIM_PIN_INCORRECT:
            std::cout << "NM_DEVICE_STATE_REASON_SIM_PIN_INCORRECT\n";
            break;
            /*
        case NM_DEVICE_STATE_REASON_NEW_ACTIVATION:
            std::cout << "NM_DEVICE_STATE_REASON_NEW_ACTIVATION\n";
            break;
        case NM_DEVICE_STATE_REASON_PARENT_CHANGED:
            std::cout << "NM_DEVICE_STATE_REASON_PARENT_CHANGED\n";
            break;
        case NM_DEVICE_STATE_REASON_PARENT_MANAGED_CHANGED:
            std::cout << "NM_DEVICE_STATE_REASON_PARENT_MANAGED_CHANGED\n";
            break;
            */
    }

}

static void nmConnectionPrint(NMConnection* con)
{   
    nm_connection_dump(con);
    /* 
    GPtrArray* hints = nullptr;
    const char* path        = nm_connection_get_path(con);
    const char* iface       = nm_connection_get_virtual_iface_name(con);
    const char* needSecrets = nm_connection_need_secrets(con, &hints);
    
    GError err;
    */
}

static int nmInfo()
{
    std::cout << "LibNM Testing:\n\n";
    //##########################  NMClient #############################
    NMClient* nmClient = nm_client_new();
    if(nmClient == nullptr)
    {
        std::cout << "Unable to create NMClient!\n";
        return -1;
    }
    std::cout << "NMClient:\n";
    nmClientPrint(nmClient);
    
    const GPtrArray * devices = nm_client_get_devices(nmClient);
    std::cout << "\nNMDevices:\n\tFound " << (devices ? devices->len : 0) << " network devices.\n";
    for(int i = 0; devices && i < devices->len; i++)
    {
        std::cout << "\nDevice " << (i + 1) << ":\n";
        NMDevice * dev = (NMDevice*) devices->pdata[i];
        if(dev != nullptr)
        {
            nmDevicePrint(dev);
            
            const GPtrArray* cons = nm_device_get_available_connections(dev);
            std::cout << "\n\t" << (cons ? cons->len : 0) << " available connections:\n";
            for(int cNum = 0; cons && cNum < cons->len; cNum++)
            {
                std::cout << "\nConnection "<< (cNum + 1) << ": ";
                nmConnectionPrint((NMConnection*) cons->pdata[cNum]);
            }
        }
    }
    GError* err = nullptr;
    DBusGConnection* bus = dbus_g_bus_get (DBUS_BUS_SYSTEM, &err);
    if(err != nullptr)
    {
        std::cout<<"DBus connection error: " << err->message << "\n";
        g_error_free(err);
        err = nullptr;
    }
    if(!bus)
    {
        std::cout << "Unable to open DBus connection!\n";
        return -1;
    }
    NMRemoteSettings* settings = nm_remote_settings_new (bus);
    if(!settings)
    {
        std::cout << "Unable to access remote connection settings!\n";
        return -1;
    }
    
    std::cout << "Reloading connections "
              << (nm_remote_settings_reload_connections(settings,&err) 
                        ? "succeeded.\n" : "failed\n");

    if(err != nullptr)
    {
        std::cout<<"Error reloading connections: " << err->message << "\n";
        g_error_free(err);
        err = nullptr;
    }
    GSList* connections = nm_remote_settings_list_connections(settings);
    if(!connections)
    {
        std::cout << "Found no saved connections!\n";
        return -1;
    }
    int savedCons = 0;
    for(GSList* iter = connections; iter != nullptr; iter=iter->next)
    {
        savedCons++;
        std::cout << "\nConnection " << savedCons << ":";
        nmConnectionPrint((NMConnection*) iter->data);
    }
    std::cout << "\nFound " << savedCons << " saved connection(s)\n";
    g_slist_free(connections);
    connections = nullptr;
}

/**
 * Loads client and device objects, and starts the signal thread.
 */
LibNMHandler::LibNMHandler()
{   
    nmInfo();
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this]()
    {
        GQuark errorQuark = nm_client_error_quark();
        nmClient = nm_client_new();
        if (nmClient == nullptr || !NM_IS_CLIENT(nmClient))
        {
            DBG("WifiEventHandler::" << __func__
                    << ": failed to connect to nmclient over dbus");
            DBG("Error: " << String(g_quark_to_string(errorQuark)));
            nmClient = nullptr;
            return;
        }
        g_object_weak_ref(G_OBJECT(nmClient), 
                (GWeakNotify) handleClientRemoved, this);
        
        MainConfigFile config;
        String iface = config.getConfigValue<String>
                (MainConfigFile::wifiInterfaceKey);
        if(iface.isNotEmpty())
        {
            DBG("WifiEventHandler::" << __func__ << ": Using wifi device " 
                    << iface);
            nmDevice = nm_client_get_device_by_iface(nmClient, 
                    iface.toRawUTF8());
        }
        else
        {
            DBG("WifiEventHandler::" << __func__ << ": No wifi device defined"
                    << "in config.json, scanning for the first"
                    << " managed wifi device");
            const GPtrArray* allDevices = nm_client_get_devices(nmClient);
            for (int i = 0; allDevices && (i < allDevices->len); i++)
            {
                NMDevice* testDev = NM_DEVICE(allDevices->pdata[i]);
                if (testDev != nullptr
                    && NM_IS_DEVICE_WIFI(testDev) 
                    && nm_device_get_managed(testDev))
                {
                    nmDevice = (NMDevice*) testDev;
                    DBG("Using wifi device " << nm_device_get_iface(testDev));
                    break;
                }
            }
        }
        if (nmDevice == nullptr || !NM_IS_DEVICE_WIFI(nmDevice))
        {
            DBG("WifiEventHandler::" << __func__ <<
                    ":  failed to find a libNM managed wifi device!");
            g_object_weak_unref(G_OBJECT(nmClient), 
                    (GWeakNotify) handleClientRemoved, this);
            nmClient = nullptr;
            nmDevice = nullptr;
            return;
        }      
        g_object_weak_ref(G_OBJECT(nmDevice), 
                (GWeakNotify) handleDeviceRemoved, this);
        
        buildAPMap();
    });
}

/**
 * Shuts down the signal thread, removing all signal handlers.
 */
LibNMHandler::~LibNMHandler()
{
    if(nmClient != nullptr)
    {
        g_object_weak_unref(G_OBJECT(nmClient), 
                (GWeakNotify) handleClientRemoved, this);
    }
   if(nmDevice != nullptr)
    {
        g_object_weak_unref(G_OBJECT(nmDevice), 
                (GWeakNotify) handleDeviceRemoved, this);
    }
    disconnectSignalHandlers();
}

/**
 * Checks if the network manager and the wifi device are available
 */
bool LibNMHandler::isWifiAvailable()
{
    return nmClient != nullptr
            && nmDevice != nullptr;
}

/**
 * @return true iff the wifi device is enabled. 
 */
bool LibNMHandler::checkWifiEnabled()
{
    bool enabled = false;
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, &enabled]()
    {
        if (isWifiAvailable())
        {
            enabled = nm_client_wireless_get_enabled(nmClient);
        }
    });
    return enabled;
}

/**
 * @return true iff the wifi device is connecting to an access point. 
 */
bool LibNMHandler::checkWifiConnecting()
{
    return activatingConn != nullptr
            && findWifiState() != NM_DEVICE_STATE_ACTIVATED;
}

/**
 * @return true iff the wifi device is connected to an access point. 
 */
bool LibNMHandler::checkWifiConnected()
{
    bool connected = false;
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, &connected]()
    {
        if (isWifiAvailable())
        {
            NMActiveConnection* activeConn 
                    = nm_client_get_primary_connection(nmClient);
            if(activeConn != nullptr)
            {
                connected = isWifiConnection(activeConn);
            }
        }
    });
    return connected;
}

/**
 * Gets the current connected access point from the network manager wifi
 * device.
 * 
 * @return the current connected access point, or the void access point if
 *         none is found.
 */
WifiAccessPoint::Ptr LibNMHandler::findConnectedAP()
{
    WifiAccessPoint::Ptr ap = nullptr;
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, &ap]()
    {
        if (isWifiAvailable() && 
            findWifiState() == NM_DEVICE_STATE_ACTIVATED)
        {
            NMAccessPoint* nmAP = nm_device_wifi_get_active_access_point
                    (NM_DEVICE_WIFI(nmDevice));
            if (nmAP == nullptr)
            {
                return;
            }
            for(WifiAccessPoint::Ptr knownAP : visibleAPs)
            {
                if(*knownAP == nmAP)
                {
                    ap = knownAP;
                    return;
                }
            }
            DBG("LibnmHandler::" << __func__ 
                    << ": Connected AP not found in visible AP list!");
        }
    });
    return ap;
}

/**
 * Gets the current connecting access point from the network manager wifi
 * device.
 * 
 * @return the current connecting access point, or nullptr if none is found.
 */
WifiAccessPoint::Ptr LibNMHandler::findConnectingAP()
{
    WifiAccessPoint::Ptr connectingAP = nullptr;
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, &connectingAP]()
    {
        if (isWifiAvailable())
        {
            if (!checkWifiConnecting())
            {
                return;
            }
            const char* path = nm_active_connection_get_specific_object
            (activatingConn);
            if(path == nullptr)
            {
                return;
            }
            NMAccessPoint* connectionNMAP =
                    nm_device_wifi_get_access_point_by_path(
                    NM_DEVICE_WIFI(nmDevice),
                    path);
            
            for(WifiAccessPoint::Ptr knownAP : visibleAPs)
            {
                if(*knownAP == connectionNMAP)
                {
                    connectingAP = knownAP;
                    return;
                }
            }
        }
    });
    return connectingAP;
}

/**
 * Returns the list of visible wifi access points.
 */
Array<WifiAccessPoint::Ptr> LibNMHandler::findVisibleAPs()
{
    Array<WifiAccessPoint::Ptr> returnedAPs;
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, &returnedAPs]()
    {
        if (isWifiAvailable())
        {
            for(WifiAccessPoint::Ptr knownAP : visibleAPs)
            {
                if(knownAP != nullptr
                        && knownAP->getSSID().isNotEmpty()
                        && knownAP->getNMAccessPoint() != nullptr)
                {
                    returnedAPs.add(knownAP);
                    for(int i = 0; i < returnedAPs.size() - 1; i++)
                    {
                        if(knownAP->sharesConnectionWith(returnedAPs[i])
                                && knownAP->getSignalStrength() 
                                > returnedAPs[i]->getSignalStrength())
                        {
                            returnedAPs[i] = knownAP;
                            returnedAPs.remove(returnedAPs.size() - 1);
                            break;
                        }
                    }
                }
            }
        }
    });
    DBG("LibNMHandler::" << __func__ << ": returning " << returnedAPs.size()
            << " APs out of " << visibleAPs.size());
    return returnedAPs;
}

/**
 * Turns the wifi device on or off.
 * 
 * @param wifiEnabled  If true, wifi will be enabled.  If false, wifi will
 *                     be disabled.
 */
void LibNMHandler::setWifiEnabled(bool wifiEnabled)
{
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, wifiEnabled]()
    {
        if (isWifiAvailable())
        {
            nm_client_wireless_set_enabled(nmClient, wifiEnabled);
        }
    });
}

/**
 * Send a request to the wifi device to scan for new access points.
 */
void LibNMHandler::requestScan()
{
    GLibSignalHandler signalHandler;
    signalHandler.gLibCallAsync([this]()
    {
        if (isWifiAvailable())
        {
            GError* error = nullptr;
            nm_device_wifi_request_scan_simple(NM_DEVICE_WIFI(nmDevice),
                    nullptr, error);
            if (error != nullptr)
            {
                DBG("LibNMHandler::requestScan: error requesting scan: "
                        << String(error->message));
                g_error_free(error);
                error = nullptr;
            }
        }
    });
}

/**
 * @return  the current state of the wifi device. 
 */
NMDeviceState LibNMHandler::findWifiState()
{
    NMDeviceState state = NM_DEVICE_STATE_UNAVAILABLE;
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, &state]()
    {
        if (isWifiAvailable())
        {
            state = nm_device_get_state(nmDevice);
        }
    });
    return state;
}

//##### Static data for LibNMHandler::initConnection: ########



//libNM callback to run after attempting to open a new connection

void LibNMHandler::handleConnectionAttempt(
        NMClient *client,
        NMActiveConnection *active,
        const char* path,
        GError *err,
        LibNMHandler* nmHandler)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    if(nmHandler == nullptr)
    {
        return;
    }
    NMAccessPoint* nmAP = nullptr;
    const char* apPath = nm_active_connection_get_specific_object(active);
    if(apPath != nullptr)
    {
        nmAP = nm_device_wifi_get_access_point_by_path
                (NM_DEVICE_WIFI(nmHandler->nmDevice),apPath);
    }
    if (err != nullptr || nmAP == nullptr)
    {
        if(err != nullptr)
        {
            gint errorCode = err->code;
            DBG("LibNMHandler::" << __func__ 
                    << ": Error=" << String(err->message));
            g_error_free(err);
            err = nullptr;
            nmHandler->invalidSecurityCallback();
            return;
        }
        nmHandler->connectionFailureCallback();
        return;
    }
    nmHandler->activatingConn = active;
    for(WifiAccessPoint::Ptr knownAP : nmHandler->visibleAPs)
    {
        if(*knownAP == nmAP)
        nmHandler->connectingCallback(knownAP);
        return;
    }
    DBG("LibNMHandler::" << __func__ 
            << ": connecting AP matches no known access point!");
}
//libNM callback run after attempting to re-open a known connection

void LibNMHandler::handleKnownConnectionAttempt(
        NMClient *client,
        NMActiveConnection *active,
        GError *err,
        LibNMHandler* nmHandler)
{
    handleConnectionAttempt(client, active, nullptr, err, nmHandler);
}

/**
 * Attempts to open a connection to an access point.
 */
void LibNMHandler::initConnection(WifiAccessPoint::Ptr toConnect, String psk)
{
    if (!isWifiAvailable() || toConnect == nullptr)
    {
        connectionFailureCallback();
        return;
    }
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this, &toConnect, &psk]()
    {
        g_assert(g_main_context_is_owner(g_main_context_default()));
        toConnect->setPsk(psk);
        NMConnection* connection = toConnect->getNMConnection();
        if(connection == nullptr)
        {
            DBG("LibNMHandler::" << __func__ << ": connection was null!");
            connectionFailureCallback(); 
            return;
        }

        //Check if the connection has been added already:
        const GPtrArray* wifiConnections 
                    = nm_device_get_available_connections(nmDevice);
        if(wifiConnections != nullptr)
        {
            for (int i = 0; i < wifiConnections->len; i++)
            {
                if(connection == wifiConnections->pdata[i])
                {
                    nm_client_activate_connection(nmClient,
                            connection,
                            nmDevice,
                            nm_object_get_path
                            (NM_OBJECT(toConnect->getNMAccessPoint())),
                            (NMClientActivateFn) handleKnownConnectionAttempt,
                            this);
                    return;
                }
            }
        }
        nm_client_add_and_activate_connection(nmClient,
                connection,
                nmDevice,
                nm_object_get_path
                (NM_OBJECT(toConnect->getNMAccessPoint())),
                (NMClientAddActivateFn) handleConnectionAttempt,
                this);
        
        //see if the new connection can be applied to any of the other access
        //points
        int connectionsAdded = 0;
        for(WifiAccessPoint::Ptr knownAP : visibleAPs)
        {
            if(knownAP != toConnect && knownAP->setNMConnection(connection))
            {
                connectionsAdded++;
            }
        }
        if(connectionsAdded > 0)
        {
            DBG("LibNMHandler::" << __func__ << ": shared new connection with "
                    << connectionsAdded << " saved access points.");
        }
        
    });
}

/**
 * Shuts down the active wifi connection.
 */
void LibNMHandler::closeActiveConnection()
{
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this]()
    {
        if (isWifiAvailable())
        {
            nm_device_disconnect(nmDevice, nullptr, nullptr);
        }
    });
}

/**
 * Shuts down any wifi connection currently being activated.
 */
void LibNMHandler::closeActivatingConnection()
{
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this]()
    {
        if (isWifiAvailable())
        {
            if (activatingConn != nullptr)
            {
                const char *ac_uuid = nm_active_connection_get_uuid
                (activatingConn);
                const GPtrArray* avail_cons =
                        nm_device_get_available_connections(nmDevice);
                for (int i = 0; avail_cons && (i < avail_cons->len); i++)
                {
                    NMRemoteConnection* candidate = (NMRemoteConnection*)
                            g_ptr_array_index(avail_cons, i);
                    const char* test_uuid = nm_connection_get_uuid
                            (NM_CONNECTION(candidate));
                    if (g_strcmp0(ac_uuid, test_uuid) == 0)
                    {
                        GError *err = nullptr;
                        nm_remote_connection_delete(candidate, nullptr, &err);
                        if (err != nullptr)
                        {
                            DBG("LibNMInterface::" << __func__
                                    << ": failed to remove active connection!");
                            DBG("LibNMInterface::" << __func__ << ": "
                                    << err->message);
                            g_error_free(err);
                            err = nullptr;
                        }
                        break;
                    }
                }
            }
        }
    });
}

/**
 * Attach all signal handlers to the wifi thread, so that they are run
 * whenever the appropriate signals occur.
 */
void LibNMHandler::connectSignalHandlers()
{
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this]()
    {
        if (isWifiAvailable())
        {
            //Signal: notifies that wifi has turned on or off
            nmClientSignalConnect("notify::" NM_CLIENT_WIRELESS_ENABLED,
                    G_CALLBACK(handleWifiEnabledChange), this);

            //Signal: notifies that wifi state has changed
            nmDeviceSignalConnect("notify::" NM_DEVICE_STATE,
                    G_CALLBACK(handleStateChange), this);

            //Signal: notifies that the active access point has changed
            nmWifiDeviceSignalConnect
                    ("notify::" NM_DEVICE_WIFI_ACTIVE_ACCESS_POINT,
                    G_CALLBACK(handleConnectionChange), this);

            //Signal: notifies that a new wifi access point is visible.
            nmWifiDeviceSignalConnect("access-point-added",
                    G_CALLBACK(handleApAdded), this);

            //Signal: notifies that a wifi access point is no longer visible.
            nmWifiDeviceSignalConnect("access-point-removed",
                    G_CALLBACK(handleApRemoved), this);
        }
    });
}

/**
 * Remove all signal handlers from the wifi thread, so that they don't 
 * register wifi updates.
 */
void LibNMHandler::disconnectSignalHandlers()
{
    if (nmDevice != nullptr && NM_IS_DEVICE_WIFI(nmDevice))
    {
        for (gulong& signalHandlerId : wifiDeviceSignalHandlers)
        {
            //DBG("LibNMHandler::"<<__func__ << ": removing wifi device handler "
            //        << String(signalHandlerId));
            g_signal_handler_disconnect(nmDevice, signalHandlerId);
        }
    }
    wifiDeviceSignalHandlers.clear();

    if (nmDevice != nullptr && NM_IS_DEVICE(nmDevice))
    {
        for (gulong& signalHandlerId : deviceSignalHandlers)
        {
            //DBG("LibNMHandler::"<<__func__ << ": removing network device handler "
            //        << String(signalHandlerId));
            g_signal_handler_disconnect(nmDevice, signalHandlerId);
        }
    }
    deviceSignalHandlers.clear();

    if (nmClient != nullptr && NM_IS_CLIENT(nmClient))
    {
        for (gulong& signalHandlerId : clientSignalHandlers)
        {
            //DBG("LibNMHandler::"<<__func__ << ": removing NM client handler "
            //        << String(signalHandlerId));
            g_signal_handler_disconnect(nmClient, signalHandlerId);
        }
        clientSignalHandlers.clear();
    }
    clientSignalHandlers.clear();
}


//Internal signal handlers:

void LibNMHandler::handleWifiEnabledChange(LibNMHandler* nmHandler)
{
//    DBG("LibNMHandler::"<<__func__ << ": data=0x"
//                << String::toHexString((unsigned long) nmHandler));
    g_assert(g_main_context_is_owner(g_main_context_default()));
    nmHandler->wifiEnablementChangeCallback(nmHandler->checkWifiEnabled());
}

void LibNMHandler::handleStateChange(LibNMHandler* nmHandler)
{
//    DBG("LibNMHandler::"<<__func__ << ": data=0x"
//                << String::toHexString((unsigned long) nmHandler));
    g_assert(g_main_context_is_owner(g_main_context_default()));
    NMDeviceState state = nm_device_get_state(nmHandler->nmDevice);
    switch (state)
    {
        case NM_DEVICE_STATE_PREPARE:
        case NM_DEVICE_STATE_CONFIG:
        case NM_DEVICE_STATE_IP_CONFIG:
        case NM_DEVICE_STATE_IP_CHECK:
        case NM_DEVICE_STATE_SECONDARIES:
        case NM_DEVICE_STATE_NEED_AUTH:
        {
            if(nmHandler->activatingConn == nullptr)
            {
                NMActiveConnection* newConnection = 
                        nm_client_get_activating_connection
                        (nmHandler->nmClient);
                if(newConnection != nullptr)
                {
                    nmHandler->activatingConn = newConnection;
                }
            }
            break;
        }
        default:
            nmHandler->activatingConn = nullptr;
    }
    nmHandler->stateUpdateCallback(state);
}

void LibNMHandler::handleApAdded(LibNMHandler* nmHandler)
{
//    DBG("LibNMHandler::" << __func__ << ": data=0x"
//                << String::toHexString((unsigned long) nmHandler));
    g_assert(g_main_context_is_owner(g_main_context_default()));
    nmHandler->buildAPMap();
    nmHandler->apUpdateCallback(nmHandler->findVisibleAPs());
}

void LibNMHandler::handleApRemoved(LibNMHandler* nmHandler)
{
//    DBG("LibNMHandler::"<<__func__ << ": data=0x"
//                << String::toHexString((unsigned long) nmHandler));
    g_assert(g_main_context_is_owner(g_main_context_default()));
    if(nmHandler->visibleAPs.isEmpty())
    {
        //no APs to remove, do nothing
        return;
    }
    if (nm_client_wireless_get_enabled(nmHandler->nmClient))
    {
        //DBG("LibNMHandler::" << __func__ << ": finding removed access points:");
        const GPtrArray* visibleAPs = nm_device_wifi_get_access_points
                (NM_DEVICE_WIFI(nmHandler->nmDevice));
        if(visibleAPs == nullptr)
        {
            DBG("LibNMHandler::" << __func__ 
                    << ": NMAccessPoint list is null, clearing AP list");
            nmHandler->visibleAPs.clear();
            return;
        }
        int removed = 0;
        Array<WifiAccessPoint::Ptr, CriticalSection> toRemove;
        for (WifiAccessPoint::Ptr savedAP : nmHandler->visibleAPs)
        {
            bool remove = true;
            if(savedAP->getNMAccessPoint() != nullptr)
            {
                for (int i = 0; i < visibleAPs->len; i++)
                {
                    if (*savedAP == (NMAccessPoint*) visibleAPs->pdata[i])
                    {
                        remove = false;
                        break;
                    }
                }
            }
            if (remove)
            {
                toRemove.add(savedAP);
                removed++;
            }
        }
        nmHandler->visibleAPs.removeValuesIn(toRemove);
        //DBG("LibNMHandler::" << __func__ << ": removed " << removed
        //        << " NMAccessPoints");
    }
    else
    {
        DBG("LibNMHandler::" << __func__ 
                << ": wifi disabled, clearing AP list");
        nmHandler->visibleAPs.clear();
    }
}

void LibNMHandler::handleConnectionChange(LibNMHandler* nmHandler)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    //DBG("LibNMHandler::" << __func__ << ": data=0x"
    //            << String::toHexString((unsigned long) nmHandler));
    WifiAccessPoint::Ptr connected = nmHandler->findConnectedAP();
    nmHandler->connectionUpdateCallback(connected);
}


    
void LibNMHandler::handleClientRemoved(LibNMHandler* self, GObject* client)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    g_assert(client == (GObject*) self->nmClient);
    self->nmClient = nullptr;
    self->clientSignalHandlers.clear();
}

void LibNMHandler::handleDeviceRemoved(LibNMHandler* self, GObject* device)
{
    g_assert(g_main_context_is_owner(g_main_context_default()));
    g_assert(device == (GObject*) self->nmDevice);
    self->nmDevice = nullptr;
    self->deviceSignalHandlers.clear();
    self->wifiDeviceSignalHandlers.clear(); 
}

/**
 * Checks if a connection belongs to the wifi device.  This function should
 * be called from the GLib event thread.
 */
bool LibNMHandler::isWifiConnection(NMActiveConnection* connection)
{
    if (connection != nullptr)
    {
        const GPtrArray* devices = nm_active_connection_get_devices(connection);
        if(devices == nullptr)
        {
            return false;
        }
        for (int i = 0; i < devices->len; i++)
        {
            if (devices->pdata[i] == nmDevice)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * Scans for all visible NMAccessPoints and build the data structure 
 * mapping WifiAccessPoint objects to their NMAccessPoints.
 */
void LibNMHandler::buildAPMap()
{
    GLibSignalHandler signalHandler;
    signalHandler.gLibCall([this]()
    {
        DBG("LibNMHandler::buildAPMap: Mapping all visible APs");
        if (!isWifiAvailable())
        {
            return;
        }   
        const GPtrArray* visibleNMAPs = nm_device_wifi_get_access_points
                (NM_DEVICE_WIFI(nmDevice));
        const GPtrArray* wifiConns = nm_device_get_available_connections
                (nmDevice);

        DBG("LibNMHandler::buildAPMap: found " 
                << String(visibleNMAPs ? visibleNMAPs->len : 0)
                << " NMAccessPoints, and " 
                << String(wifiConns ? wifiConns->len : 0)
                << " saved wifi connections");
        if (visibleNMAPs == nullptr)
        {
            return;
        }
        for (int apNum = 0; apNum < visibleNMAPs->len; apNum++)
        {
            NMAccessPoint* nmAP = 
                    (NMAccessPoint*) visibleNMAPs->pdata[apNum];
            
            //Skip NMAccessPoints that are already saved
            bool matchingNMAP = false;
            for(WifiAccessPoint::Ptr knownAP : visibleAPs)
            {
                if(*knownAP == nmAP)
                {
                    matchingNMAP = true;
                    break;
                }
            }
            if(matchingNMAP)
            {
                break;
            }
            
            //look for a matching valid connection
            NMConnection* apSavedConn = nullptr;
            if(wifiConns != nullptr)
            {
                GSList* wifiConnList = nullptr;
                for (int i = 0; i < wifiConns->len; i++)
                {
                    wifiConnList = g_slist_prepend(wifiConnList,
                            wifiConns->pdata[i]);
                }
                GSList* matchingConns = nm_access_point_filter_connections
                        (nmAP, wifiConnList);
                        g_slist_free(wifiConnList);
                for (GSList* iter = matchingConns; iter != nullptr;
                     iter = iter->next)
                {
                    NMConnection* conn
                            = (NMConnection*) iter->data;
                            GError * error = nullptr;
                    if (nm_connection_verify(conn, &error))
                    {
                        apSavedConn = conn;
                        break;
                    }
                    else if (error != nullptr)
                    {
                        DBG("LibNMHandler: invalid connection: "
                                << String(error->message));
                                g_error_free(error);
                                error = nullptr;
                    }
                }
                g_slist_free(matchingConns);
            }
            WifiAccessPoint::Ptr wifiAP 
                    = new WifiAccessPoint(nmAP, apSavedConn);
            if(wifiAP == nullptr)
            {
                DBG("LibNMHandler::buildAPMap: failed to create wifi AP!");
            }
            else
            {
                visibleAPs.add(wifiAP);
                DBG("LibNMHandler::buildAPMap: Added AP #"
                        << visibleAPs.size() << " with SSID "
                        << wifiAP->getSSID()
                        << (wifiAP->hasSavedConnection() ? " (saved)"
                        : " (new)"));
            }
        }
    });
}

/**
 * Connects a signal handler to the network manager client.
 */
gulong LibNMHandler::nmClientSignalConnect(
        const char* signal,
        GCallback signalHandler,
        gpointer callbackData)
{
    if (nmClient == nullptr)
    {
        return 0;
    }
    gulong handlerId = g_signal_connect_swapped
            (nmClient, signal, signalHandler, callbackData);
    if(handlerId > 0)
    {
    //    DBG("LibNMHandler::" << __func__ << " : connected signal " << signal
    //            << " with ID " << handlerId << "and data=0x"
    //            << String::toHexString((unsigned long) callbackData));
        clientSignalHandlers.add(handlerId);
    }
    else
    {
        DBG("LibNMHandler::" << __func__ << " : failed to connect signal " 
                << signal);
    }
    return handlerId;
}

/**
 * Connects a signal handler to the generic wlan0 device.
 */
gulong LibNMHandler::nmDeviceSignalConnect(
        const char* signal,
        GCallback signalHandler,
        gpointer callbackData)
{
    if (nmDevice == nullptr)
    {
        return 0;
    }
    gulong handlerId = g_signal_connect_swapped 
            (nmDevice, signal, signalHandler, callbackData);
    if(handlerId > 0)
    {
//        DBG("LibNMHandler::" << __func__ << " : connected signal " << signal
//                << " with ID " << handlerId << "and data=0x"
//                << String::toHexString((unsigned long) callbackData));
        deviceSignalHandlers.add(handlerId);
    }
    else
    {
        DBG("LibNMHandler::" << __func__ << " : failed to connect signal " 
                << signal);
    }
    return handlerId;
}

/**
 * Connects a signal handler to the wifi device wlan0.
 */
gulong LibNMHandler::nmWifiDeviceSignalConnect(
        const char* signal,
        GCallback signalHandler,
        gpointer callbackData)
{
    if (nmDevice == nullptr)
    {
        return 0;
    }
    gulong handlerId = g_signal_connect_swapped
            (NM_DEVICE_WIFI(nmDevice), signal, signalHandler, callbackData);
    if(handlerId > 0)
    {
    //    DBG("LibNMHandler::" << __func__ << " : connected signal " << signal
    //            << " with ID " << handlerId << "and data=0x"
    //            << String::toHexString((unsigned long) callbackData));
        wifiDeviceSignalHandlers.add(handlerId);
    }
    else
    {
        DBG("LibNMHandler::" << __func__ << " : failed to connect signal " 
                << signal);
    }
    return handlerId;
}
