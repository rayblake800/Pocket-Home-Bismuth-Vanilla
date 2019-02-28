#include "Utils.h"
#include "IconSliderComponent.h"

/*
 * Creates a slider with a default range of 0-100.
 */
IconSliderComponent::IconSliderComponent(const juce::Identifier& imageKey) :
lowIcon(imageKey, 0, juce::RectanglePlacement::stretchToFit),
highIcon(imageKey, 1, juce::RectanglePlacement::stretchToFit)
{
#if JUCE_DEBUG
    setName("IconSliderComponent");
#endif
    using juce::Slider;
    slider.setSliderStyle(Slider::LinearHorizontal);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setRange(0, 100);
    addAndMakeVisible(slider);
    addAndMakeVisible(lowIcon);
    addAndMakeVisible(highIcon);
}

/*
 * Changes the slider's stored value.
 */
void IconSliderComponent::setValue
(double newValue, juce::NotificationType notification)
{
    slider.setValue(newValue, notification);
}

/*
 * Gets the slider's stored value.
 */
double IconSliderComponent::getValue() const
{
    return slider.getValue();
}

/*
 * Subscribes a slider listener object to changes in the slider's value.
 */
void IconSliderComponent::addListener(juce::Slider::Listener* listener)
{
    slider.addListener(listener);
}

/*
 * Compares a raw slider pointer to this object.
 */
bool IconSliderComponent::operator==(const juce::Slider* sliderPtr) const
{
    return sliderPtr == &slider;
}

/* 
 * Sets the slider value's limits.
 */
void IconSliderComponent::setRange(double newMinimum, double newMaximum,
        double newInterval)
{
    slider.setRange(newMinimum, newMaximum, newInterval);
}

/*
 * Updates the icons and slider to fit the component bounds.
 */
void IconSliderComponent::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    lowIcon.setBounds(bounds.withWidth(bounds.getHeight()));
    highIcon.setBounds(bounds.withLeft(bounds.getRight() - bounds.getHeight()));
    slider.setBounds(bounds.reduced(bounds.getHeight(), 0));
}
