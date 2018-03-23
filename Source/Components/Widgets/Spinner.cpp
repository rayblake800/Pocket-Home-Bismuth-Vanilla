#include "AssetFiles.h"
#include "Spinner.h"

Spinner::Spinner(int secondsToTimeout) :
WindowFocusedTimer("SpinnerFrame"),
spinnerImage(RectanglePlacement::fillDestination),
timeout(secondsToTimeout)
{
#    if JUCE_DEBUG
    setName("spinner");
#    endif
    StringArray spinnerImgPaths = {
                                   "spinner0.svg",
                                   "spinner1.svg",
                                   "spinner2.svg",
                                   "spinner3.svg",
                                   "spinner4.svg",
                                   "spinner5.svg",
                                   "spinner6.svg",
                                   "spinner7.svg"
    };
    for (const String& path : spinnerImgPaths)
    {
        spinnerImages.add(AssetFiles::loadImageAsset(path));
    }
    spinnerImage.setImage(spinnerImages[0]);
    addAndMakeVisible(spinnerImage);
}

Spinner::~Spinner()
{
    stopTimer();
}

/**
 * Changes the color of the spinner icon.
 */
void Spinner::setColour(Colour spinnerColour)
{
    spinnerImage.setColour(DrawableImageComponent::imageColour0Id,
            spinnerColour);
}

/**
 * Sets the internal image component's bounds within the Spinner.
 */
void Spinner::setImageBounds(Rectangle<int> imageBounds)
{
    spinnerImage.setBounds(imageBounds);
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
 * Sets spinner image bounds to match the component.
 */
void Spinner::resized()
{
    spinnerImage.setBounds(getLocalBounds());
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
    else
    {
        imageIndex = (imageIndex + 1) % spinnerImages.size();
        spinnerImage.setImage(spinnerImages[imageIndex]);
        startTimer(timerInterval);
    }
}


