#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_Connection_Saved_Module.h
 *
 * @brief  Reads and removes saved Wifi connections.
 */
#include "Wifi_Module.h"
#include "Wifi_LibNM_DBus_SavedConnectionLoader.h"

namespace Wifi
{
    namespace Connection { namespace Saved { class Module; } }
    class Resource;
    class AccessPoint;
    namespace LibNM
    {
        class Connection;
        class AccessPoint;
    }
}

/**
 * @brief  Handles saved Wifi connection data loaded from NetworkManager.
 *
 *  Saved::Module loads LibNM::DBus saved connection objects, reads the last
 * time that they were active, and allows them to be deleted. The Saved::Module
 * is responsible for setting and updating the saved connection status stored
 * by AccessPoint objects.
 */
class Wifi::Connection::Saved::Module : public Wifi::Module
{
public:
    /**
     * @brief  Loads all connections saved by NetworkManager.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    Module(Resource& parentResource);

    virtual ~Module() { }

    /**
     * @brief  Checks if NetworkManager has a saved connection that is
     *         compatible with an access point.
     *
     * @param toCheck  The access point to check against the list of saved
     *                 connections.
     *
     * @return  Whether a saved connection exists that uses that access point.
     */
    bool hasSavedConnection(const AccessPoint toCheck);

    /**
     * @brief  Checks if NetworkManager has a saved connection that is
     *         compatible with an access point.
     *
     * @param toCheck  The access point to check against the list of saved
     *                 connections.
     *
     * @return  Whether a saved connection exists that uses that access point.
     */
    bool hasSavedConnection(const LibNM::AccessPoint toCheck) const;

    /**
     * @brief  Attempts to find and return a saved connection object that is
     *         compatible with an access point.
     *
     * @param connectionAP  The access point that would be used with the
     *                      requested saved connection.
     *
     * @return              The first saved connection found that is compatible
     *                      with connectionAP, or a null Connection object if
     *                      no match is found.
     */
    LibNM::Connection getSavedConnection(const AccessPoint connectionAP);

    /**
     * @brief  Finds the last time the system was fully connected to a
     *         particular wifi access point's connection.
     *
     * @param connectionAP  A Wifi access point object to search for in saved
     *                      connection records.
     *
     * @return              The last time the system was connected using a
     *                      connection compatible with connectionAP, or the
     *                      Unix epoch if no saved connection was found.
     */
    juce::Time lastConnectionTime(const AccessPoint connectionAP);

    /**
     * @brief  Removes all saved network connections that match a particular
     *         access point.
     *
     * @param toRemove  The access point used to select saved connections to
     *                  delete.
     */
    void removeSavedConnection(AccessPoint toRemove);

    /**
     * @brief  Updates an access point's saved data to store whether any
     *         matching saved connection exists and the most recent time any
     *         matching saved connection was connected.
     *
     * @param toUpdate  An AccessPoint object to update.
     *
     *  Because AccessPoint objects share data, these updates will be applied
     * to every equivalent AccessPoint object.
     */
    void updateSavedAPData(AccessPoint toUpdate);

    /**
     * @brief  Gets all saved connections compatible with a particular
     *         AccessPoint object.
     *
     * This method must be called within the LibNM::ThreadResource.
     *
     * @param toMatch  The access point to search for.
     *
     * @return         All saved connections that could have used the provided
     *                 AccessPoint parameter.
     */
    juce::Array<LibNM::DBus::SavedConnection> getMatchingConnections
    (const Wifi::AccessPoint toMatch);

    /**
     * @brief  Gets all saved connections compatible with a particular
     *         LibNM::AccessPoint object.
     *
     * This method must be called within the LibNM::ThreadResource.
     *
     * @param toMatch  The access point to search for.
     *
     * @return         All saved connections that could have used the provided
     *                 AccessPoint parameter.
     */
    juce::Array<LibNM::DBus::SavedConnection> getMatchingConnections
    (const Wifi::LibNM::AccessPoint toMatch);

private:
    // Reads and removes saved network connections:
    LibNM::DBus::SavedConnectionLoader savedConnections;
};
