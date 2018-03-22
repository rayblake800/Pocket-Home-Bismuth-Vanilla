/**
 * @file Audio.h
 * 
 * Audio provides functions for initializing system audio and viewing/changing
 * system volume.
 */
#pragma once

namespace Audio
{
    /**
     * Initializes system audio.
     * 
     * @return true iff audio was successfully initialized.
     */
    bool initAudio();
    
    /**
     * Gets the system's volume level.
     * 
     * @return system audio volume level, as a percentage of the maximum
     *          volume.
     */
    int getVolumePercent();
    
    /**
     * Changes the system audio volume level.
     * 
     * @param volumePercent  The volume level, which will be constrained to
     *                        values between 0 and 100.
     */
    void setVolume(int volumePercent);
};