#pragma once
/**
 * @file  LibNM/APData/SSID.h
 *
 * @brief  Stores a Wifi access point's ID.
 */

#include "Util/Nullable.h"
#include "JuceHeader.h"
#include <glib.h>

namespace LibNM { class SSID; }

class LibNM::SSID
{
public:
    /**
     * @brief  Creates this SSID copying another SSID.
     *
     * @param toCopy  The other SSID to copy.
     */
    SSID(const SSID& toCopy);

    /**
     * @brief  Creates this SSID from a SSID byte string.
     *
     * @param toCopy  The SSID byte string this object will copy.
     */
    SSID(GByteArray* toCopy);

    /**
     * @brief  Frees all SSID data on destruction.
     */
    virtual ~SSID();

    /**
     * @brief  Gets a text representation of this SSID.
     *
     * @return  The SSID, as a text string suitable for display or debugging. 
     */
    juce::String toString() const;

    /**
     * @brief  Gets this SSID's internal SSID byte string.
     *
     * @return  The SSID's exact value, as a byte array that may contain 
     *          unprintable characters.
     */
    GByteArray* getByteArray() const;

    /**
     * @brief  Assigns another SSID's data to this SSID.
     *
     * @param toCopy  Another SSID this SSID object should copy.
     *
     * @return        This SSID object.
     */
    SSID& operator=(const SSID& toCopy);

    /**
     * @brief  Assigns a copy of an SSID bytestring to this SSID.
     *
     * @param toAssign  A SSID byte string that will be used to replace this
     *                  object's data.
     *
     * @return          This SSID object.
     */
    SSID& operator=(GByteArray* toAssign);

    /**
     * @brief  Checks if two SSIDs are equivalent.
     *
     * @param rhs  Another SSID to compare with this one.
     *
     * @return     Whether both SSIDs have identical stored values. 
     */
    bool operator==(const SSID& rhs) const;

    /**
     * @brief  Checks if two SSIDs are not equivalent.
     *
     * @param rhs  Another SSID to compare with this one.
     *
     * @return     Whether both SSIDs do not have identical stored values. 
     */
    bool operator!=(const SSID& rhs) const;

    /**
     * @brief  Compares this object with another SSID alphabetically.
     *
     * @param rhs  Another SSID to compare with this one.
     *
     * @return     Whether this SSID's byte string comes before rhs.
     */
    bool operator<(const SSID& rhs) const;

    /**
     * @brief  Checks if a SSID and a raw SSID byte string are equivalent.
     *
     * @param rhs  A raw SSID bytestring to compare to this object.
     *
     * @return     Whether this object's internal byte string matches rhs.
     */
    bool operator==(GByteArray* rhs) const;


    /**
     * @brief  Checks if a SSID and a raw SSID byte string are not equivalent.
     *
     * @param rhs  A raw SSID bytestring to compare to this object.
     *
     * @return     Whether this object's internal byte string does not match 
     *             rhs.
     */
    bool operator!=(GByteArray* rhs) const;

private:
    /* Stores the SSID value as a byte string. */
    GByteArray* ssidBytes = nullptr;
};
