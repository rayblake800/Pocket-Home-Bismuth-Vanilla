#define WIFI_IMPLEMENTATION
#include "Wifi_Module.h"
#include "Wifi_Resource.h"

/*
 * Connects the module to its Resource.
 */
Wifi::Module::Module(Resource& parentResource) :
    SharedResource::Modular::Module<Resource>(parentResource) { }

/*
 * Packages an asynchronous action so that it will check if the Wifi::Resource 
 * instance that created it is still valid.  
 */
std::function<void()> Wifi::Module::buildAsyncFunction
(SharedResource::LockType lockType, std::function<void()> action)
{
    return getResource().buildAsyncFunction(lockType, action);
}
