#include "AssetFiles.h"
#include "Spinner.h"
#include "ComponentConfigFile.h"

Spinner::Spinner(int secondsToTimeout) :
WindowFocusedTimer("SpinnerFrame"),
ConfigurableImageComponent(
ComponentConfigFile::spinnerKey,
0,RectanglePlacement::fillDestination),
timeout(secondsToTimeout)
{
#    if JUCE_DEBUG
    setName("spinner");
#    endif
    ComponentConfigFile config;
    numImages = config.getComponentSettings(ComponentConfigFile::spinnerKey)
            .getAssetFiles().size();
}

/**
 * Disables animation when losing visibility, enables animation when
 * gaining visibility.
 */
void Spinner::visibilityChanged()
{
    if (isVisible())
    {
        startTimer(timerInterval);
    }
    else
    {
        stopTimer();
    }
}

/**
 * Shows the next frame of the spinner animation, and disables the spinner if
 * runtime exceeds the timeout period.
 */
void Spinner::timerCallback()
{
    runtime += getTimerInterval();
    if (runtime > timeout && timeout > 0)
    {
        runtime = 0;
        setVisible(false);
    }
    else if(numImages > 0)
    {
        imageIndex = (imageIndex + 1) % numImages;
        setImageAssetIndex(imageIndex);
        startTimer(timerInterval);
    }
}


