#define WIFI_IMPLEMENTATION
#include "Wifi_Signal_ClientModule.h"
#include "Wifi_Resource.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_Connection_Control_Module.h"
#include "Wifi_Device_Module.h"
#include "Wifi_LibNM_Client.h"
#include "Wifi_LibNM_DeviceWifi.h"
#include "Wifi_LibNM_ContextTest.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Wifi::Signal::ClientModule::";
#endif

// Connects the module to its Resource.
Wifi::Signal::ClientModule::ClientModule(Resource& parentResource) :
    Wifi::Module(parentResource) { }


// Starts tracking the LibNM::ThreadResource's Client object.
void Wifi::Signal::ClientModule::connect()
{
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this, nmThread]()
    {
        LibNM::Client networkClient = nmThread->getClient();
        connectAllSignals(networkClient);
    });
}


// Stops tracking the LibNM::ThreadResource's Client object.
void Wifi::Signal::ClientModule::disconnect()
{
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->call([this]() { disconnectAll(); } );
}


// Notifies the Wifi::Device::Module when wireless networking is enabled or
// disabled.
void Wifi::Signal::ClientModule::wirelessStateChange(bool wifiEnabled)
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread
            = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write,
            [this, wifiEnabled]()
    {
        DBG(dbgPrefix << __func__ << ": Wireless state changed to "
                << (wifiEnabled ? "enabled" : "disabled"));
        if (!wifiEnabled)
        {
            Connection::Control::Module* connectionController
                    = getSiblingModule<Connection::Control::Module>();
            connectionController->signalWifiDisabled();
        }

        LibNM::Thread::Module* nmThread
                = getSiblingModule<LibNM::Thread::Module>();
        const bool validWifiDevice = !nmThread->getWifiDevice().isNull();
        Device::Module* deviceTracker = getSiblingModule<Device::Module>();
        deviceTracker->updateDeviceState(validWifiDevice, wifiEnabled);
    });
}
