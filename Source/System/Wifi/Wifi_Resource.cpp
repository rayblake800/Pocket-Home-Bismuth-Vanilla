#define WIFI_IMPLEMENTATION
#include "Wifi_Resource.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_Device_Module.h"
#include "Wifi_Connection_Saved_Module.h"
#include "Wifi_Connection_Record_Module.h"
#include "Wifi_Connection_Control_Module.h"
#include "Wifi_APList_Module.h"
#include "Wifi_Signal_DeviceModule.h"
#include "Wifi_Signal_APModule.h"
#include "Wifi_Signal_ClientModule.h"
#include "Wifi_Device_Listener.h"
#include "Wifi_FocusUpdater.h"

// The SharedResource::Resource object instance key:
const juce::Identifier Wifi::Resource::resourceKey = "Wifi::Resource";

// Updates connection records when window focus is regained:
static std::unique_ptr<Wifi::FocusUpdater> focusUpdater;

//##################### Shared resource modules: #############################

// Runs the LibNM Wifi thread:
static std::unique_ptr<Wifi::LibNM::Thread::Module> nmThread;

// Tracks the state of the active Wifi device:
static std::unique_ptr<Wifi::Device::Module> deviceTracker;

// Accesses saved connections:
static std::unique_ptr<Wifi::Connection::Saved::Module> savedConnections;

// Tracks the list of visible Wifi access points:
static std::unique_ptr<Wifi::APList::Module> apList;

// Keeps a record of Wifi connection events:
static std::unique_ptr<Wifi::Connection::Record::Module> connectionRecord;

// Opens and closes Wifi connections:
static std::unique_ptr<Wifi::Connection::Control::Module> connectionControl;

// Handles all signals from LibNM AccessPoint objects.
static std::unique_ptr<Wifi::Signal::APModule> apSignalHandler;

// Handles all signals from the LibNM thread resource's DeviceWifi object.
static std::unique_ptr<Wifi::Signal::DeviceModule> deviceSignalHandler;

// Handles all signals from the LibNM thread resource's Client object.
static std::unique_ptr<Wifi::Signal::ClientModule> clientSignalHandler;

//#####################  Resource function definitions:  #######################
// Initializes all Wifi resource modules.
Wifi::Resource::Resource() : SharedResource::Modular::Resource<>(resourceKey)
{
    DBG("Wifi::Resource::Resource: Initializing Wifi resources.");
    nmThread.reset(new LibNM::Thread::Module(*this));
    deviceTracker.reset(new Device::Module(*this));
    savedConnections.reset(new Connection::Saved::Module(*this));
    apList.reset(new APList::Module(*this));
    connectionControl.reset(new Connection::Control::Module(*this));
    connectionRecord.reset(new Connection::Record::Module(*this));
    apSignalHandler.reset(new Signal::APModule(*this));
    deviceSignalHandler.reset(new Signal::DeviceModule(*this));
    clientSignalHandler.reset(new Signal::ClientModule(*this));

    DBG("Wifi::Resource::Resource: Finishing init in the LibNM thread");
    nmThread->startResourceThread();
    nmThread->call([this]()
    {
        deviceTracker->updateDeviceState();
        apList->updateAllAccessPoints();
        connectionRecord->updateRecords();
        clientSignalHandler->connect();
        deviceSignalHandler->connect();
        focusUpdater.reset(new FocusUpdater);
        DBG("Wifi::Resource::Resource: Finished initializing Wifi resources.");
    });
}


// Shuts down all Wifi resource modules.
Wifi::Resource::~Resource()
{
    clientSignalHandler.reset(nullptr);
    deviceSignalHandler.reset(nullptr);
    apSignalHandler.reset(nullptr);
    connectionRecord.reset(nullptr);
    connectionControl.reset(nullptr);
    apList.reset(nullptr);
    savedConnections.reset(nullptr);
    deviceTracker.reset(nullptr);
    nmThread.reset(nullptr);
    focusUpdater.reset(nullptr);
}


// Packages an asynchronous action so that it will check if the Wifi::Resource
// instance that created it is still valid.
std::function<void()> Wifi::Resource::buildAsyncFunction
(SharedResource::LockType lockType, std::function<void()> action)
{
    return SharedResource::Resource::buildAsyncFunction(lockType, action);
}


//###############  Module access function specializations:  ####################

// Older compiler versions will not allow template specializations outside of
// the original namespace.
namespace SharedResource { namespace Modular {

template<> template<> Wifi::LibNM::Thread::Module*
SharedResource::Modular::Resource<>::getModule<Wifi::LibNM::Thread::Module>()
{
    return nmThread.get();
}

template<> template<> Wifi::Device::Module*
SharedResource::Modular::Resource<>::getModule<Wifi::Device::Module>()
{
    return deviceTracker.get();
}

template<> template<> Wifi::Connection::Saved::Module*
SharedResource::Modular::Resource<>::getModule
        <Wifi::Connection::Saved::Module>()
{
    return savedConnections.get();
}

template<> template<> Wifi::Connection::Record::Module*
SharedResource::Modular::Resource<>::getModule
        <Wifi::Connection::Record::Module>()
{
    return connectionRecord.get();
}

template<> template<> Wifi::Connection::Control::Module*
SharedResource::Modular::Resource<>::getModule
        <Wifi::Connection::Control::Module>()
{
    return connectionControl.get();
}

template<> template<> Wifi::APList::Module*
SharedResource::Modular::Resource<>::getModule <Wifi::APList::Module>()
{
    return apList.get();
}

template<> template<> Wifi::Signal::APModule*
SharedResource::Modular::Resource<>::getModule <Wifi::Signal::APModule>()
{
    return apSignalHandler.get();
}

template<> template<> Wifi::Signal::DeviceModule*
SharedResource::Modular::Resource<>::getModule <Wifi::Signal::DeviceModule>()
{
    return deviceSignalHandler.get();
}

template<> template<> Wifi::Signal::ClientModule*
SharedResource::Modular::Resource<>::getModule <Wifi::Signal::ClientModule>()
{
    return clientSignalHandler.get();
}

template<> template<> const Wifi::LibNM::Thread::Module*
SharedResource::Modular::Resource<>::getModule
        <const Wifi::LibNM::Thread::Module>()
{
    return nmThread.get();
}

template<> template<> const Wifi::Device::Module*
SharedResource::Modular::Resource<>::getModule <const Wifi::Device::Module>()
{
    return deviceTracker.get();
}

template<> template<> const Wifi::Connection::Saved::Module*
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Connection::Saved::Module>()
{
    return savedConnections.get();
}

template<> template<> const Wifi::Connection::Record::Module*
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Connection::Record::Module>()
{
    return connectionRecord.get();
}

template<> template<> const Wifi::Connection::Control::Module*
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Connection::Control::Module>()
{
    return connectionControl.get();
}

template<> template<> const Wifi::APList::Module*
SharedResource::Modular::Resource<>::getModule<const Wifi::APList::Module>()
{
    return apList.get();
}

template<> template<> const Wifi::Signal::APModule*
SharedResource::Modular::Resource<>::getModule <const Wifi::Signal::APModule>()
{
    return apSignalHandler.get();
}

template<> template<> const Wifi::Signal::DeviceModule*
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Signal::DeviceModule>()
{
    return deviceSignalHandler.get();
}

template<> template<> const Wifi::Signal::ClientModule*
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Signal::ClientModule>()
{
    return clientSignalHandler.get();
}

} } // Close SharedResource::Modular namespace
