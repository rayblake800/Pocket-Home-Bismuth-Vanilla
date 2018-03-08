#include "../Utils.h"
#include "IconSliderComponent.h"

IconSliderComponent::IconSliderComponent
(String lowImgAsset, String highImgAsset) :
lowIcon(lowImgAsset,RectanglePlacement::stretchToFit),
highIcon(highImgAsset,RectanglePlacement::stretchToFit)
{
    
#if JUCE_DEBUG
    setName("IconSliderComponent");
#endif
    slider.setSliderStyle(Slider::LinearHorizontal);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setRange(0, 100);
    addAndMakeVisible(slider);
    addAndMakeVisible(lowIcon);
    addAndMakeVisible(highIcon);
}

IconSliderComponent::~IconSliderComponent() { }

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
    Rectangle<int> bounds = getLocalBounds();
    lowIcon.setBounds(bounds.withWidth(bounds.getHeight()));
    highIcon.setBounds(bounds.withLeft(bounds.getRight()-bounds.getHeight()));
    slider.setBounds(bounds.reduced(bounds.getHeight(),0));
//    sliderLayout.setItemLayout(0, height, height, height);
//    sliderLayout.setItemLayout(1, 50, -1.0, -1.0);
//    sliderLayout.setItemLayout(2, height, height, height);
//
//    Component * parts[] = {&lowIcon, &slider, &highIcon};
//    sliderLayout.layOutComponents(parts, 3, getX(), getY(),
//            width, height, false, true);
}
