#define WIFI_IMPLEMENTATION
#include "Wifi_Signal_APModule.h"
#include "Wifi_Resource.h"
#include "Wifi_APList_Module.h"
#include "Wifi_LibNM_Thread_Module.h"
#include "Wifi_AccessPoint.h"
#include "Wifi_LibNM_AccessPoint.h"
#include "Wifi_LibNM_ContextTest.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Wifi::Signal::APModule::";
#endif

/*
 * Connects the module to its Resource.
 */
Wifi::Signal::APModule::APModule(Resource& parentResource) :
    Wifi::Module(parentResource) { }

/*
 * Passes signal strength updates to the access point list.
 */
void Wifi::Signal::APModule::signalStrengthChanged
(const LibNM::AccessPoint updatedAP, unsigned int newStrength)
{
    ASSERT_NM_CONTEXT;
    LibNM::Thread::Module* nmThread = getSiblingModule<LibNM::Thread::Module>();
    nmThread->lockForAsyncCallback(SharedResource::LockType::write,
            [this, updatedAP, newStrength]()
    {
        DBG(dbgPrefix << __func__ << ": Signal strength change for " 
                << updatedAP.getSSIDText() << " to strength " 
                << (int) newStrength);
        APList::Module* apList = getSiblingModule<APList::Module>();
        AccessPoint updated = apList->getAccessPoint(updatedAP.generateHash());
        apList->updateSignalStrength(updated);
    });
}
