#pragma once

/**
 * @file Audio.h
 * 
 * @brief Provides functions for initializing system audio and viewing/changing
 *        system volume.
 */

namespace Audio
{
    /**
     * @brief  Opens a connection to Alsa audio.
     *
     * This is used as a quick fix to a PocketCHIP bug where touch screen 
     * presses cause buzzing when no application has opened Alsa.
     * 
     * @return  Whether the Alsa connection was successfully initialized.
     */
    bool chipAudioInit();
    
    /**
     * @brief Gets the system's volume level.
     * 
     * @return  The system audio volume level, as a percentage of the maximum
     *          volume.
     */
    int getVolumePercent();
    
    /**
     * @brief  Changes the system audio volume level.
     * 
     * @param volumePercent  The volume level, which will be constrained to
     *                       values between 0 and 100.
     */
    void setVolume(const int volumePercent);
};
