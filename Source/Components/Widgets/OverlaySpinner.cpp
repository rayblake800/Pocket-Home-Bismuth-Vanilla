#include "Utils.h"
#include "OverlaySpinner.h"

OverlaySpinner::OverlaySpinner(int secondsToTimeout) :
spinner(secondsToTimeout)
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("OverlaySpinner");
#    endif
    loadingText.setText("", NotificationType::dontSendNotification);
    loadingText.setJustificationType(Justification::centred);
    loadingText.setColour(Label::textColourId, findColour(textColourId));
    addAndMakeVisible(loadingText);
    addAndMakeVisible(spinner);
}

/**
 * Sets text to display below the spinner
 */
void OverlaySpinner::setLoadingText(juce::String newText)
{
    using namespace juce;
    loadingText.setText(newText, NotificationType::dontSendNotification);
}

/**
 * Fills in the overlay background.
 */
void OverlaySpinner::paint(juce::Graphics &g)
{
    g.fillAll(findColour(backgroundColourId));
}

/**
 * Update the spinner and loading text to fit overlay bounds.
 */
void OverlaySpinner::resized()
{
    using namespace juce;
    Rectangle<int> bounds = getLocalBounds();
    int spinnerSize = bounds.getHeight() / 10;
    spinner.setBounds(bounds.withSizeKeepingCentre(spinnerSize, spinnerSize));
    Rectangle<int> textBounds = bounds.withHeight(spinnerSize)
            .withY(bounds.getX() + bounds.getHeight()*2 / 3);
    loadingText.setBounds(textBounds);
}


