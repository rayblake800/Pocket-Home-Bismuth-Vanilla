/**
 * @file IconSliderComponent.h
 * 
 * IconSliderComponent is a horizontal slider that draws icons on
 * its left and right sides.
 */
 
#pragma once
#include "DrawableImageComponent.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class IconSliderComponent : public Component {
public:
    /**
     * @param lowImgAsset asset name to use for the icon shown to
     * the left of the slider
     * @param highImgAsset asset name to use for the icon shown to
     * the left of the slider
     */
    IconSliderComponent(String lowImgAsset, String highImgAsset);
    virtual ~IconSliderComponent();

    /**
     * 
     * Change the slider's stored value
     * @param newValue sets the slider position.  0 sets the slider
     * to the far left, 100 sets the slider to the far right.
     * @param notification determines if listeners will be notified of
     * this change.
     */
    void setValue(double newValue,
            NotificationType notification = sendNotificationAsync);

    /**
     * @return the current slider position value, between 0 and 100.
     */
    double getValue() const;

    /**
     * @param listener will receive updates whenever the slider value
     * changes.
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
     * Update the icons and slider to fit the component bounds.
     */
    void resized() override;

    DrawableImageComponent lowIcon;
    DrawableImageComponent highIcon;
    Slider slider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconSliderComponent)
};
