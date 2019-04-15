#pragma once
/**
 * @file  Wifi_TestUtils_Waiting.h
 *
 * @brief  Defines how long Wifi test classes should wait for specific Wifi
 *         actions to occur.
 */

#include "JuceHeader.h"

namespace Wifi
{
    class AccessPoint;
    namespace Connection { class Event; }
    namespace LibNM { class APHash; }

    namespace TestUtils
    {
        namespace Waiting
        {

            /**
             * @brief  Waits until multiple visible access points have been
             *         registered to the APList.
             *
             * @return  True if multiple access points were found, false if the
             *          function times out before multiple access points
             *          appeared.
             */
            bool waitForAccessPoints();

            /**
             * @brief  Waits for the wifi device to detect a specific access
             *         point.
             *
             * @param apHash  The hash value identifying an access point.
             *
             * @return  The matching access point once it has loaded, or a null
             *          access point if no match is found before the function
             *          times out.
             */
            AccessPoint waitForAccessPoint(const LibNM::APHash apHash);

            /**
             * @brief  Waits for the next Wifi connection event to be
             *         registered.
             *
             * @param startTime  The time that the connection event should come
             *                   after.
             *
             * @return           The first event found that is registered after
             *                   the start time, or a null event if the function
             *                   times out before an event appears.
             */
            Connection::Event waitForNextConnectionEvent
                    (const juce::Time startTime);

        }
    }
}
