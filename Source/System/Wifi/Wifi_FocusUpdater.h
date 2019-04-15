#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_FocusUpdater.h
 *
 * @brief  Updates Wifi records when window focus is regained.
 */

#include "Windows_FocusListener.h"

namespace Wifi { class FocusUpdater; }

/**
 * @brief  Ensures the tracked active Wifi connection is up to date whenever
 *         window focus is regained.
 *
 *  The Wifi thread only remains active while the main application window is
 * focused. When window focus is regained, the FocusUpdater ensures that the
 * Connection::Record::Module checks for missed connection changes.
 */
class Wifi::FocusUpdater : public Windows::FocusListener
{
public:
    FocusUpdater() { }

    virtual ~FocusUpdater() { }

private:
    /**
     * @brief  Scans for missed Wifi connection events when window focus is
     *         regained.
     */
    virtual void windowFocusGained() override;
};
