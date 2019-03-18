#include "Hardware_Audio.h"
#include "Util_Commands.h"
#include "JuceHeader.h"
#include <exception>
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
 * @brief  Interrupts ALSA initialization when an ALSA library function returns
 *         an error value.
 */
struct ALSAException : public std::exception
{
public:
    /**
     * @brief  Creates a new exception for an ALSA error code
     *
     * @param errorCode     The error code returned by an ALSA library function.
     *
     * @param errorMessage  An optional brief description of the error.
     */
    ALSAException(const int errorCode, const juce::String errorMessage = "")
    {
#ifdef JUCE_DEBUG
        error = dbgPrefix;
        error += __func__;
        error += ": ALSA ERROR: ";
        error += errorMessage;
        error += " Error code " + juce::String(errorCode) + ": ";
        error += snd_strerror(errorCode);
#endif
    }

    /**
     * @brief  Gets a string describing the error.
     *
     * @return  A string containing the error message, and an explanation of the
     *          error code.
     */
    virtual const char* what() const noexcept override
    {
        return error.toRawUTF8();
    }

private:
    /* The description of the ALSA error: */
    juce::String error;
};

/**
 * @brief  Throws an ALSAException if the function result code it is given is
 *         an error code.
 *
 * @param resultCode      The result code returned by an ALSA library function.
 *
 * @param errorMessage    An optional error message to pass to the exception if
 *                        the result code is an error code.
 *
 * @throws ALSAException  Whenever resultCode is less than zero, indicating an
 *                        error.
 */
static void alsaCheck(const int resultCode,
        const juce::String errorMessage = "")
{
    if(resultCode < 0)
    {
        throw ALSAException(resultCode, errorMessage);
    }
}

/*
 * Opens a connection to Alsa audio. This is used as a quick fix to a PocketCHIP
 * bug where touch screen presses cause buzzing when no application has opened 
 * Alsa.
 */
bool Hardware::Audio::chipAudioInit()
{
    snd_pcm_hw_params_t* hardwareParams = nullptr;
    snd_pcm_sw_params_t* softwareParams = nullptr;

    snd_pcm_hw_params_malloc(&hardwareParams);
    snd_pcm_sw_params_malloc(&softwareParams);

    snd_pcm_t* pcmHandle;
    bool initSucceeded = false;
    try
    {
        alsaCheck(snd_pcm_open(&pcmHandle, deviceName, SND_PCM_STREAM_PLAYBACK,
                0),
                "Can't open audio device");
        alsaCheck(snd_pcm_hw_params_any(pcmHandle, hardwareParams),
                "Can't initialize hardware parameter structure");
        alsaCheck(snd_pcm_hw_params_set_access(pcmHandle, hardwareParams, 
                SND_PCM_ACCESS_RW_INTERLEAVED),
                "Can't set access type");
        alsaCheck(snd_pcm_hw_params_set_format(pcmHandle, hardwareParams,
                SND_PCM_FORMAT_S16_LE),
                "Can't set sample format");
        int alsaFrequency = 44100;
        alsaCheck(snd_pcm_hw_params_set_rate_near(pcmHandle,
                hardwareParams, (unsigned int*)& alsaFrequency, 0),
                "Can't set sample rate");
        alsaCheck(snd_pcm_hw_params_set_channels(pcmHandle, hardwareParams,
                alsaChannels),
                "Can't set channel count");
        snd_pcm_uframes_t frames = alsaBufferSize;
        alsaCheck(snd_pcm_hw_params_set_buffer_size_near(pcmHandle,
                hardwareParams, &frames),
                "Can't set buffer size");
        snd_pcm_hw_params_get_buffer_size(hardwareParams, &frames);
        DBG(dbgPrefix << __func__ << ": ALSA Buffer size: 4096 samples");
        alsaCheck(snd_pcm_hw_params(pcmHandle, hardwareParams),
                "Can't set parameters");
        alsaCheck(snd_pcm_prepare(pcmHandle),
                "Can't prepare audio interface for use");
        /* Stop PCM device and drop pending frames */
        snd_pcm_drain(pcmHandle);
        initSucceeded = true;
    }
    catch(const ALSAException& e)
    {
        DBG(e.what());
    }
    snd_pcm_hw_params_free(hardwareParams);
    snd_pcm_sw_params_free(softwareParams);
    hardwareParams = nullptr;
    softwareParams = nullptr;
    return initSucceeded;
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
