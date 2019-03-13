#include "Hardware_Audio.h"
#include "Util_Commands.h"
#include "JuceHeader.h"
#include <alsa/asoundlib.h>

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Hardware::Audio::";
#endif

/* ALSA buffer size in samples: */
static const constexpr int alsaBufferSize = 4096;
/* ALSA channel count: */
static const constexpr int alsaChannels = 2;
/* Audio device to use: */
static const constexpr char* deviceName = "default";

// Preprocessor macros are ugly, but it's better than dozens of lines of 
// near-identical error checks.

/**
 * @brief  Runs an ALSA audio function, printing an error and returning false if
 *         the function fails.
 *
 * @param alsaFunction  An ALSA audio function that returns an error code.
 *
 * @param errorMessage  An error message to print before the result code 
 *                      description if the result code is an error code.
 */
#define ALSA_FN(alsaFunction, errorMessage)                                \
resultCode = alsaFunction;                                                 \
if(resultCode < 0)                                                         \
{                                                                          \
    DBG(dbgPrefix << __func__ << ": ALSA ERROR: " << errorMessage << ": "  \
                << snd_strerror(resultCode));                              \
    return false;                                                          \
}                                                                          \

/*
 * Opens a connection to Alsa audio. This is used as a quick fix to a PocketCHIP
 * bug where touch screen presses cause buzzing when no application has opened 
 * Alsa.
 */
bool Hardware::Audio::chipAudioInit()
{
    snd_pcm_hw_params_t* hardwareParams;
    snd_pcm_sw_params_t* softwareParams;

    snd_pcm_hw_params_malloc(&hardwareParams);
    snd_pcm_sw_params_malloc(&softwareParams);

    int resultCode;
    snd_pcm_t* pcmHandle;
    ALSA_FN(snd_pcm_open(&pcmHandle, deviceName, SND_PCM_STREAM_PLAYBACK, 0),
            "Can't open audio device");
    ALSA_FN(snd_pcm_hw_params_any(pcmHandle, hardwareParams),
            "Can't initialize hardware parameter structure");
    ALSA_FN(snd_pcm_hw_params_set_access(pcmHandle, hardwareParams, 
            SND_PCM_ACCESS_RW_INTERLEAVED),
            "Can't set access type");
    ALSA_FN(snd_pcm_hw_params_set_format(pcmHandle, hardwareParams,
            SND_PCM_FORMAT_S16_LE),
            "Can't set sample format");
    int alsaFrequency = 44100;
    ALSA_FN(snd_pcm_hw_params_set_rate_near(pcmHandle,
            hardwareParams, (unsigned int*)& alsaFrequency, 0),
            "Can't set sample rate");
    ALSA_FN(snd_pcm_hw_params_set_channels(pcmHandle, hardwareParams,
            alsaChannels),
            "Can't set channel count");
    snd_pcm_uframes_t frames = alsaBufferSize;
    ALSA_FN(snd_pcm_hw_params_set_buffer_size_near(pcmHandle, hardwareParams,
            &frames),
            "Can't set buffer size");
    snd_pcm_hw_params_get_buffer_size(hardwareParams, &frames);
    DBG(dbgPrefix << __func__ << ": ALSA Buffer size: 4096 samples");
    ALSA_FN(snd_pcm_hw_params(pcmHandle, hardwareParams),
            "Can't set parameters");
    snd_pcm_hw_params_free(hardwareParams);
    snd_pcm_sw_params_free(softwareParams);
    ALSA_FN(snd_pcm_prepare(pcmHandle),
            "Can't prepare audio interface for use");
    /* Stop PCM device and drop pending frames */
    snd_pcm_drain(pcmHandle);
    return true;
    // Someone needs to donate some more vowels to whoever wrote the ALSA sound
    // library...
}

/*
 * Gets the system's volume level.
 */
int Hardware::Audio::getVolumePercent()
{
    Util::Commands systemCommands;
    juce::String volume = systemCommands.runTextCommand(
            Util::CommandTypes::Text::getVolume);
    DBG(dbgPrefix << __func__ << ": System volume=" << volume);
    return volume.getIntValue();
}

/*
 * Changes the system audio volume level.
 */
void Hardware::Audio::setVolume(int volumePercent)
{
    juce::String volumeArg(volumePercent);
    volumeArg += "%";
    Util::Commands systemCommands;
    systemCommands.runActionCommand(Util::CommandTypes::Action::setVolume,
            volumeArg);
}
