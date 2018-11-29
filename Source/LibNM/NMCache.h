#pragma once
#include "GLib/ThreadHandler.h"
#include "LibNM/LibNM.h"
#include "LibNM/APHash.h"
#include "LibNM/NMObjects/Client.h"
#include "LibNM/NMObjects/DeviceWifi.h"
#include "LibNM/NMObjects/ActiveConnection.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/NMThread.h"
#include <map>

/**
 * @file  LibNM/NMCache.h
 *
 * @brief  Creates and stores relevant LibNM objects, and allows them to be
 *         accessed within the GLib wifi context within the NMThread.
 */
class LibNM::NMCache : private GLib::ThreadHandler
{
public:
    /**
     * @brief  Initializes the cache of saved NMObjects within the NMThread.
     */
    NMCache();

    /**
     * @brief 
     */
    virtual ~NMCache();

    /**
     * @brief 
     *
     * @return 
     */
    Client& getClient();

    /**
     * @brief 
     *
     * @return 
     */
    DeviceWifi& getWifiDevice();

    /**
     * @brief 
     *
     * @return 
     */
    ActiveConnection& getActiveConnection();

    /**
     * @brief 
     *
     * @return 
     */
    std::multimap<APHash, AccessPoint>& getAccessPoints();

    /**
     * @brief 
     *
     * @param hashValue
     *
     * @return 
     */
    juce::Array<AccessPoint> getAccessPoints(const APHash hashValue);

    /**
     * @brief 
     *
     * @param hashValue
     *
     * @return 
     */
    AccessPoint getAccessPoint(const APHash hashValue);

    /**
     * @brief 
     *
     * @param newActiveConnection
     */
    void setActiveConnection(ActiveConnection newActiveConnection);

    /**
     * @brief 
     *
     * @param newAP
     */
    void addAccessPoint(AccessPoint newAP);

    /**
     * @brief 
     *
     * @param removedAP
     */
    void removeAccessPoint(AccessPoint removedAP);

private:
    /* */
    Client networkClient;

    /* */
    DeviceWifi wifiDevice;

    /* */
    ActiveConnection activeConnection;

    /* */
    std::multimap<APHash, AccessPoint> accessPoint;
};
