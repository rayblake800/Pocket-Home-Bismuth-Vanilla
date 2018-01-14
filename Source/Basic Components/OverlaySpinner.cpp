#include "../Utils.h"
#include "OverlaySpinner.h"

OverlaySpinner::OverlaySpinner() : overlaySpinnerTimer(this)
{
    Array<String> spinnerImgPaths{"wait0.png", "wait1.png", "wait2.png",
        "wait3.png", "wait4.png", "wait5.png", "wait6.png", "wait7.png"};
    for (const String& path : spinnerImgPaths)
    {
        Image image = createImageFromFile(assetFile(path));
        spinnerImages.add(image);
    }

    spinnerImage = new ImageComponent();
    spinnerImage->setImage(spinnerImages[0], RectanglePlacement::centred);
    addAndMakeVisible(spinnerImage);

    loadingText.setText("", NotificationType::dontSendNotification);
    loadingText.setJustificationType(Justification::centred);
    loadingText.setColour(Label::textColourId,Colours::black);
    addAndMakeVisible(loadingText);
}

OverlaySpinner::~OverlaySpinner()
{
    overlaySpinnerTimer.stopTimer();
}

void OverlaySpinner::paint(Graphics &g)
{
    g.fillAll(Colour((uint8) 254, 255, 255, (uint8) 201));
}

void OverlaySpinner::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    int spinnerSize = bounds.getHeight() / 10;
    spinnerImage->setBounds(
            (bounds.getWidth() - spinnerSize) / 2,
            (bounds.getHeight() - spinnerSize) / 2,
            spinnerSize,
            spinnerSize
            );
    Rectangle<int> textBounds = bounds.withHeight(spinnerSize)
            .withY(bounds.getX() + bounds.getHeight()*2 / 3);
    Font scaledFont = loadingText.getFont().withHeight(spinnerSize - 2);
    loadingText.setFont(scaledFont);
    loadingText.setBounds(textBounds);

}

void OverlaySpinner::setVisible(bool shouldBeVisible)
{
    DBG("Show spinner");
    Component::setVisible(shouldBeVisible);

    if (shouldBeVisible)
    {
        overlaySpinnerTimer.startTimer(500);
    } else
    {
        overlaySpinnerTimer.stopTimer();
    }
}

void OverlaySpinner::setLoadingText(String newText)
{
    loadingText.setText(newText, NotificationType::dontSendNotification);
}

void OverlaySpinner::OverlaySpinnerTimer::timerCallback()
{
    if (overlaySpinner != nullptr)
    {
        ImageComponent* spinnerImage = overlaySpinner->spinnerImage;
        const Array<Image>& spinnerImages = overlaySpinner->spinnerImages;

        // change image
        imageIndex = (imageIndex + 1) % spinnerImages.size();
        spinnerImage->setImage(spinnerImages[imageIndex]);

        // check timeout
        runtime += getTimerInterval();
        if (runtime > timeout)
        {
            runtime = 0;
            overlaySpinner->setVisible(false);
            stopTimer();
        }
    }
}

