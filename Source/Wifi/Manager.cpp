#define WIFI_IMPLEMENTATION
#include "Wifi/Manager.h"
#include "Wifi/AccessPointList/APListWriter.h"

/*
 * Initializes the LibNM thread resource, then creates and connects all Wifi
 * signal handlers.
 */
Wifi::Manager::Manager()
{
    DBG("Wifi::Manager::Manager: Initializing Wifi resources.");
    nmThread.startThread();
    nmThread.call([this]()
    {
        apListReader.reset(new APListReader);

        Wifi::APListWriter listInit;
        listInit.updateAllAccessPoints();

        recordReader.reset(new Connection::RecordReader);

        clientSignalHandler.reset(new ClientSignalHandler());
        deviceSignalHandler.reset(new DeviceSignalHandler());
        clientSignalHandler->connect();
        deviceSignalHandler->connect();
    });
    DBG("Wifi::Manager::Manager: Finished initializing Wifi resources.");
}

/*
 * Destroys all Wifi signal handlers before the LibNM thread resource can be
 * destroyed.
 */
Wifi::Manager::~Manager()
{
    nmThread.call([this]()
    {
        clientSignalHandler->disconnect();
        deviceSignalHandler->disconnect();
        clientSignalHandler.reset(nullptr);
        deviceSignalHandler.reset(nullptr);
        recordReader.reset(nullptr);
        apListReader.reset(nullptr);
    });
    nmThread.stopThread();
}

/*
 * Disables signal handlers while the application does not have window focus.
 */
void Wifi::Manager::windowFocusLost()
{
    clientSignalHandler->disconnect();
    deviceSignalHandler->disconnect();
}

/*
 * Enables signal handlers when the application regains window focus.
 */
void Wifi::Manager::windowFocusGained()
{
    clientSignalHandler->connect();
    deviceSignalHandler->connect();
}
