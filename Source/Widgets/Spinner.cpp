#include "AssetFiles.h"
#include "Spinner.h"
#include "Theme_Image_Component.h"
#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_JSONKeys.h"

Spinner::Spinner(int secondsToTimeout) :
WindowFocusedTimer("SpinnerFrame"),
Theme::Image::Component<DrawableImageComponent>(Theme::Image::JSONKeys::spinner,
        0, juce::RectanglePlacement::fillDestination),
timeout(secondsToTimeout)
{
#    if JUCE_DEBUG
    setName("spinner");
#    endif
    Theme::Image::ConfigFile config;
    numImages = config.getAssetList(Theme::Image::JSONKeys::spinner)
            .getImageFiles().size();
}

/*
 * Disables animation when losing visibility, and enables animation when gaining
 * visibility.
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

/*
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
