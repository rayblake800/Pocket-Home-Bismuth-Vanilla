#pragma once
/**
 * @file  LibNM/APData/APMode.h
 *
 * @brief  Enumerates all Wifi access point types.
 *
 *  Any combination of APMode and SecurityType values may be used as bitflags in
 * a single byte.
 */
namespace LibNM
{
    enum class APMode
    {
        unknown        = 0b0000000,
        adhoc          = 0b0000001,
        infrastructure = 0b0000010,
        hotspot        = 0b0000100
    };
}
