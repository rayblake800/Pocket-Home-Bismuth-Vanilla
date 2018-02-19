#include "../Utils.h"
#include "Spinner.h"

Spinner::Spinner(int secondsToTimeout) :
spinnerImage(RectanglePlacement::fillDestination),
timeout(secondsToTimeout)
{
    Array<String> spinnerImgPaths = {
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
        spinnerImages.add(createImageFromFile(assetFile(path)));
    }
    spinnerImage.setImage(spinnerImages[0]);
    addAndMakeVisible(spinnerImage);
}

Spinner::~Spinner()
{
    stopTimer();
}

void Spinner::setColour(Colour spinnerColour) {
    spinnerImage.setColour(DrawableImageComponent::imageColour0Id,spinnerColour);
 }

/**
 * Set the internal image component's bounds within the Spinner
 */
void Spinner::setImageBounds(Rectangle<int> imageBounds)
{
    spinnerImage.setBounds(imageBounds);
}

/**
 * Disable animation when losing visibility, enable animation when
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

void Spinner::resized()
{
    spinnerImage.setBounds(getLocalBounds());
}

/**
 * Show the next frame of the spinner animation, and disable if runtime 
 * exceeds the timeout period.
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


