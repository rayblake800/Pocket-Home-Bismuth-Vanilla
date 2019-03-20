#ifndef WIFI_IMPLEMENTATION
  #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file Wifi_Connection_Saved_Reader.h
 *
 * @brief  Looks up saved Wifi connection data, and searches for saved 
 *         connections matching specific access points.
 */

#include "SharedResource_Modular_Handler.h"

namespace Wifi 
{ 
    class Resource;
    class AccessPoint;
    namespace LibNM { class Connection; }
    namespace Connection 
    { 
        namespace Saved 
        { 
            class Reader; 
            class Module;
        }   
    } 
}


/**
 * @brief  Connects to the Connection::Saved::Module to read saved Wifi
 *         connection data.
 */
class Wifi::Connection::Saved::Reader : 
    public SharedResource::Modular::Handler<Resource, Module>
{
public:
    /**
     * @brief  Initializes the Wifi::Resource object if necessary.
     */
    Reader();

    virtual ~Reader() { }
    
    /**
     * @brief  Checks if NetworkManager has a saved connection that is
     *         compatible with an access point.
     *
     * @param toCheck  The access point to check against the list of saved
     *                 connections.
     *
     * @return         Whether a saved connection exists that uses that access 
     *                 point. 
     */
    bool hasSavedConnection(const AccessPoint toCheck);

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
     * @param connectionAP  A Wifi access point object to search for in
     *                      saved connection records.
     *
     * @return              The last time the system was connected using a 
     *                      connection compatible with connectionAP, or the 
     *                      Unix epoch if no saved connection was found.
     */
    juce::Time lastConnectionTime(const AccessPoint connectionAP);
};
