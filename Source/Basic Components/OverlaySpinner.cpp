#include "../Utils.h"
#include "OverlaySpinner.h"

OverlaySpinner::OverlaySpinner(int secondsToTimeout) :
Spinner(secondsToTimeout)
{
    
#if JUCE_DEBUG
    setName("OverlaySpinner");
#endif
    loadingText.setText("", NotificationType::dontSendNotification);
    loadingText.setJustificationType(Justification::centred);
    loadingText.setColour(Label::textColourId, findColour(textColourId));
    addAndMakeVisible(loadingText);
}

OverlaySpinner::~OverlaySpinner() { }

void OverlaySpinner::paint(Graphics &g)
{
    g.fillAll(findColour(backgroundColourId));
}

void OverlaySpinner::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    int spinnerSize = bounds.getHeight() / 10;
    setImageBounds(bounds.withSizeKeepingCentre(spinnerSize,spinnerSize));
    Rectangle<int> textBounds = bounds.withHeight(spinnerSize)
            .withY(bounds.getX() + bounds.getHeight()*2 / 3);
    loadingText.setBounds(textBounds);
}


void OverlaySpinner::setLoadingText(String newText)
{
    loadingText.setText(newText, NotificationType::dontSendNotification);
}


