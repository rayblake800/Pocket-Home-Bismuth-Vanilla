#define WIFI_IMPLEMENTATION
#include "Wifi_FocusUpdater.h"
#include "Wifi_Connection_Record_Handler.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Wifi::FocusUpdater::";
#endif

// Scans for missed Wifi connection events when window focus is regained.
void Wifi::FocusUpdater::windowFocusGained()
{
    DBG(dbgPrefix << __func__ << ": Updating connection records:");
    Connection::Record::Handler recordHandler;
    recordHandler.updateRecords();
}
