#include "../Utils.h"
#include "../Settings/SettingsPageComponent.h"
#include "IconSliderComponent.h"

IconSliderComponent::IconSliderComponent(Drawable* low, Drawable* high)
{
    slider.setSliderStyle(Slider::LinearHorizontal);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setRange(0, 100);
    lowIcon=new DrawableButton("low", DrawableButton::ImageFitted);
    lowIcon->setImages(low);
    highIcon=new DrawableButton("low", DrawableButton::ImageFitted);
    highIcon->setImages(high);
    addAndMakeVisible(slider);
    addAndMakeVisible(lowIcon);
    addAndMakeVisible(highIcon);
}

IconSliderComponent::~IconSliderComponent()
{
}

void IconSliderComponent::setValue
(double newValue, NotificationType notification)
{
    slider.setValue(newValue, notification);
}

double IconSliderComponent::getValue() const
{
    return slider.getValue();
}

void IconSliderComponent::addListener(Slider::Listener* listener)
{
    slider.addListener(listener);
}

bool IconSliderComponent::ownsSlider(Slider * sliderPtr)
{
    return sliderPtr == &slider;
}

void IconSliderComponent::resized()
{
  auto bounds = getLocalBounds();
  auto bh = bounds.getHeight();
  auto bw = bounds.getWidth();

  sliderLayout.setItemLayout(0, bh, bh, bh);
  sliderLayout.setItemLayout(1, 50, -1.0, -1.0);
  sliderLayout.setItemLayout(2, bh, bh, bh);

  Component *parts[] = { lowIcon, &slider, highIcon };
  sliderLayout.layOutComponents(parts, 3, bounds.getX(), bounds.getY(), bw, bh, false, true);
}
