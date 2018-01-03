#include "../Utils.h"
#include "../Settings/SettingsPageComponent.h"
#include "IconSliderComponent.h"

IconSliderComponent::IconSliderComponent(const Drawable& low,
        const Drawable& high)
{
    lowIcon=low.createCopy();
    highIcon=high.createCopy();
    setSliderStyle(Slider::LinearHorizontal);
    setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(lowIcon);
    addAndMakeVisible(highIcon);
    setRange(0, 100);
}

IconSliderComponent::~IconSliderComponent()
{
}

void IconSliderComponent::resized()
{   
    int height = getHeight();
    int width = getWidth();
    sliderLayout.setItemLayout(0, height, height, height);
    sliderLayout.setItemLayout(1, 50, -1.0, -1.0);
    sliderLayout.setItemLayout(2, height, height, height);

    Component * parts[] = {lowIcon, this, highIcon};
    sliderLayout.layOutComponents(parts, 3, 0, 0, width, height, false, true);
}
