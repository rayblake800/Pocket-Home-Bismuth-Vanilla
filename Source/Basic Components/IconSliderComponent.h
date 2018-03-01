/**
 * @file IconSliderComponent.h
 * 
 * TODO: finish documentation.
 */
#pragma once
#include "DrawableImageComponent.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class IconSliderComponent : public Component {
public:
    /**
     * 
     * @param lowImgAsset
     * @param highImgAsset
     */
    IconSliderComponent(String lowImgAsset, String highImgAsset);
    virtual ~IconSliderComponent();

    /**
     * 
     * @param newValue
     * @param notification
     */
    void setValue(double newValue,
            NotificationType notification = sendNotificationAsync);

    /**
     * 
     * @return 
     */
    double getValue() const;

    /**
     * 
     * @param listener
     */
    void addListener(Slider::Listener* listener);

    /**
     * Use this to determine which IconSliderComponent is responsible for
     * a slider callback.
     * @param sliderPtr a pointer to a slider object
     * @return true iff slider belongs to this IconSliderComponent
     */
    bool ownsSlider(Slider * sliderPtr);
private:
    /**
     * 
     */
    void resized() override;

    DrawableImageComponent lowIcon;
    DrawableImageComponent highIcon;
    StretchableLayoutManager sliderLayout;
    Slider slider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconSliderComponent)
};
