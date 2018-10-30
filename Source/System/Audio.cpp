#include <alsa/asoundlib.h>
#include "JuceHeader.h"
#include "SystemCommands.h"
#include "Audio.h"

/* ALSA buffer size in samples: */
static const constexpr int defaultBufferSize = 4096;
snd_pcm_t *g_alsa_playback_handle = 0;

/*
 * Opens a connection to Alsa audio. This is used as a quick fix to a PocketCHIP
 * bug where touch screen presses cause buzzing when no application has opened 
 * Alsa.
 */
bool Audio::chipAudioInit()
{
    int err;
    int freq = 44100, channels = 2;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;

    snd_pcm_hw_params_malloc(&hw_params);
    snd_pcm_sw_params_malloc(&sw_params);
    err = snd_pcm_open(&g_alsa_playback_handle, "default",
            SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't open audio device: "
                << snd_strerror(err));
        return false;
    }
    DBG("Audio::" << __func__ << ": Opened Audio Device");
    err = snd_pcm_hw_params_any(g_alsa_playback_handle, hw_params);
    if (err < 0)
    {
        DBG("Audio::" << __func__ << ": ALSA ERROR: "
                << "Can't initialize hardware parameter structure: "
                << snd_strerror(err));
        return false;
    }
    err = snd_pcm_hw_params_set_access(g_alsa_playback_handle, hw_params,
            SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't set access type: "
                << snd_strerror(err));
        return false;
    }
    err = snd_pcm_hw_params_set_format(g_alsa_playback_handle, hw_params,
            SND_PCM_FORMAT_S16_LE);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't set sample format :"
                << snd_strerror(err));
        return false;
    }
    err = snd_pcm_hw_params_set_rate_near(g_alsa_playback_handle, hw_params,
            (unsigned int*) &freq, 0);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't set sample rate: "
                << snd_strerror(err));
        return false;
    }
    DBG("Audio::" << __func__ << ": ALSA Sample rate: "
            << freq);
    err = snd_pcm_hw_params_set_channels(g_alsa_playback_handle, hw_params,
            channels);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't set channel count: "
                << snd_strerror(err));
        return false;
    }
    snd_pcm_uframes_t frames;
    frames = defaultBufferSize;
    err = snd_pcm_hw_params_set_buffer_size_near(g_alsa_playback_handle,
            hw_params, &frames);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't set buffer size: "
                << snd_strerror(err));
        return false;
    }
    snd_pcm_hw_params_get_buffer_size(hw_params, &frames);
    DBG("Audio::" << __func__
            << ": ALSA Buffer size: 4096 samples");
    err = snd_pcm_hw_params(g_alsa_playback_handle, hw_params);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't set parameters: " << snd_strerror(err));
        return false;
    }
    snd_pcm_hw_params_free(hw_params);
    snd_pcm_sw_params_free(sw_params);

    err = snd_pcm_prepare(g_alsa_playback_handle);
    if (err < 0)
    {
        DBG("Audio::" << __func__
                << ": ALSA ERROR: Can't prepare audio interface for use: "
                << snd_strerror(err));
        return false;
    }

    /* Stop PCM device and drop pending frames */
    snd_pcm_drain(g_alsa_playback_handle);
    return true;
}

/*
 * Gets the system's volume level.
 */
int Audio::getVolumePercent()
{
    using namespace juce;
    SystemCommands systemCommands;
    String volume = systemCommands.runTextCommand
        (SystemCommands::TextCommand::getVolume);
    DBG("Audio::" << __func__ << ": System volume=" << volume);
    return volume.getIntValue();
}

/*
 * Changes the system audio volume level.
 */
void Audio::setVolume(int volumePercent)
{
    using namespace juce;
    String volumeArg(volumePercent);
    volumeArg += "%";
    SystemCommands systemCommands;
    systemCommands.runActionCommand
        (SystemCommands::ActionCommand::setVolume, volumeArg);
}
