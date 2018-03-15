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
     * @return true iff audio was successfully initialized.
     */
    bool initAudio();
    
    /**
     * @return system audio volume level, as a percentage of the maximum
     *  volume.
     */
    int getVolumePercent();
    
    /**
     * Change the system audio volume.
     * @param volumePercent volume level, from 0 to 100
     */
    void setVolume(int volumePercent);
};