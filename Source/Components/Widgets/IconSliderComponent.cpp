#include "Utils.h"
#include "IconSliderComponent.h"

IconSliderComponent::IconSliderComponent
(String lowImgAsset, String highImgAsset) :
lowIcon(lowImgAsset, RectanglePlacement::stretchToFit),
highIcon(highImgAsset, RectanglePlacement::stretchToFit)
{

#    if JUCE_DEBUG
    setName("IconSliderComponent");
#    endif
    slider.setSliderStyle(Slider::LinearHorizontal);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setRange(0, 100);
    addAndMakeVisible(slider);
    addAndMakeVisible(lowIcon);
    addAndMakeVisible(highIcon);
}

IconSliderComponent::~IconSliderComponent() { }

/**
 * Change the slider's stored value
 */
void IconSliderComponent::setValue
(double newValue, NotificationType notification)
{
    slider.setValue(newValue, notification);
}

/**
 * @return the current slider position value, between 0 and 100.
 */
double IconSliderComponent::getValue() const
{
    return slider.getValue();
}

/**
 * @param listener will receive updates whenever the slider value
 * changes.
 */
void IconSliderComponent::addListener(Slider::Listener* listener)
{
    slider.addListener(listener);
}

/**
 * Use this to determine which IconSliderComponent is responsible for
 * a slider callback.
 */
bool IconSliderComponent::ownsSlider(Slider * sliderPtr)
{
    return sliderPtr == &slider;
}

/** Sets the limits that the slider's value can take.
 * @param newMinimum the lowest value allowed
 * @param newMaximum the highest value allowed
 * @param newInterval the steps in which the value is allowed to increase 
 * - if this is not zero, the value will always be (newMinimum + 
 * (newInterval * an integer)).
 */
void IconSliderComponent::setRange(double newMinimum,
        double newMaximum,
        double newInterval)
{
    slider.setRange(newMinimum, newMaximum, newInterval);
}

/**
 * Update the icons and slider to fit the component bounds.
 */
void IconSliderComponent::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    lowIcon.setBounds(bounds.withWidth(bounds.getHeight()));
    highIcon.setBounds(bounds.withLeft(bounds.getRight() - bounds.getHeight()));
    slider.setBounds(bounds.reduced(bounds.getHeight(), 0));
}
