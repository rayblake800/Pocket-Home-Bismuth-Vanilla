#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_LibNM_Signal_APHandler.h
 *
 * @brief  Provides a basis for classes that should receive updates when access
 *         point signal strengths change.
 */

#include "SharedResource_Handler.h"
#include "GLib_Signal_Handler.h"

namespace Wifi
{
    namespace LibNM
    {
        namespace Signal { class APHandler; }
        class AccessPoint;
    }
}


/**
 * @brief  Receives signal strength update notifications from connected
 *         LibNM::AccessPoint objects.
 */
class Wifi::LibNM::Signal::APHandler :
        public GLib::Signal::Handler<AccessPoint>
{
public:
    APHandler() { }

    virtual ~APHandler() { }

    /**
     * @brief  Disconnects the signal handler from all access points.
     */
    void disconnect();

    /**
     * @brief  Subscribes to signal strength signals from a single AccessPoint.
     *
     * @param source  A LibNM::AccessPoint signal source.
     */
    virtual void connectAllSignals(const AccessPoint source) final override;

private:
    /**
     * @brief  Notifies the signal handler of a change in access point signal
     *         strength.
     *
     *  This function does nothing by default. APHandler classes should override
     * it to handle signal strength change events.
     *
     * @param updatedAP    The LibNM::AccessPoint that had its signal strength
     *                     change.
     *
     * @param newStrength  The new strength of the access point, as a value
     *                     between zero and one hundred, inclusive.
     */
    virtual void signalStrengthChanged(const AccessPoint updatedAP,
            unsigned int newStrength);

    /**
     * @brief  Builds signalStrengthChanged() calls from generic property
     *         change notifications.
     *
     * @param source    The NMAccessPoint responsible for the property change
     *                  signal.
     *
     * @param property  The updated property type.
     */
    virtual void propertyChanged
    (const AccessPoint source, juce::String property) override;
};
