#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi_Resource.h
 *
 * @brief  Manages shared Wifi data and controls.
 */

#include "SharedResource_Modular_Resource.h"

namespace Wifi { class Resource; }

class Wifi::Resource : public SharedResource::Modular::Resource<>
{
public:
    /* The SharedResource::Resource object instance key: */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes all Wifi resource modules and starts the Wifi thread.
     */
    Resource();

    /**
     * @brief  Shuts down all Wifi resource modules.
     */
    virtual ~Resource();
};

/* Resource module class declarations: */
namespace Wifi
{
    namespace LibNM { namespace Thread { class Module; } }
    namespace Device { class Module; }
    namespace APList { class Module; }
    namespace Connection
    {
        namespace Saved { class Module; }
        namespace Record { class Module; }
        namespace Control { class Module; }
    }
}

/* getModule() template specializations: */
namespace SharedResource { namespace Modular {

template<> template<> Wifi::LibNM::Thread::Module*
SharedResource::Modular::Resource<>::getModule<Wifi::LibNM::Thread::Module>();

template<> template<> Wifi::Device::Module* 
SharedResource::Modular::Resource<>::getModule<Wifi::Device::Module>();

template<> template<> Wifi::Connection::Saved::Module* 
SharedResource::Modular::Resource<>::getModule
        <Wifi::Connection::Saved::Module>();

template<> template<> Wifi::Connection::Record::Module* 
SharedResource::Modular::Resource<>::getModule
        <Wifi::Connection::Record::Module>();

template<> template<> Wifi::Connection::Control::Module* 
SharedResource::Modular::Resource<>::getModule
        <Wifi::Connection::Control::Module>();

template<> template<> Wifi::APList::Module* 
SharedResource::Modular::Resource<>::getModule <Wifi::APList::Module>();

template<> template<> const Wifi::LibNM::Thread::Module*
SharedResource::Modular::Resource<>::getModule
        <const Wifi::LibNM::Thread::Module>();

template<> template<> const Wifi::Device::Module* 
SharedResource::Modular::Resource<>::getModule <const Wifi::Device::Module>();

template<> template<> const Wifi::Connection::Saved::Module* 
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Connection::Saved::Module>();

template<> template<> const Wifi::Connection::Record::Module* 
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Connection::Record::Module>();

template<> template<> const Wifi::Connection::Control::Module* 
SharedResource::Modular::Resource<>::getModule
        <const Wifi::Connection::Control::Module>();

template<> template<> const Wifi::APList::Module* 
SharedResource::Modular::Resource<>::getModule<const Wifi::APList::Module>();

} } // Close SharedResource::Modular namespace
