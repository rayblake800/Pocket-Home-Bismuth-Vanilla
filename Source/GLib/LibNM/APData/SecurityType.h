#pragma once
#include "LibNM/LibNM.h"

/**
 * @file  LibNM/APData/SecurityType.h
 *
 * @brief  Enumerates all basic Wifi security types.
 *
 *  Any combination of APMode and SecurityType values may be used as bitflags in
 * a single byte.
 */
enum class LibNM::SecurityType
{
    unsecured      = 0b0001000,
    securedWEP     = 0b0010000,
    securedWPA     = 0b0100000,
    securedRSN     = 0b1000000,
};
