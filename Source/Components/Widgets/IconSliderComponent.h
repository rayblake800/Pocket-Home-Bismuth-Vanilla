#pragma once
#include "DrawableImageComponent.h"
#include "JuceHeader.h"

/**
 * @file IconSliderComponent.h
 * 
 * IconSliderComponent is a horizontal slider that draws icons on
 * its left and right sides.
 */

class IconSliderComponent : public Component
{
public:
    /**
     * Creates a slider with a default range of 0-100.
     * 
     * @param lowImgAsset  The asset name to use for the icon shown to the left 
     *                      of the slider.
     * 
     * @param highImgAsset The asset name to use for the icon shown to the 
     *                      right of the slider.
     */
    IconSliderComponent(String lowImgAsset, String highImgAsset);

    virtual ~IconSliderComponent() { }

    /**
     * Change the slider's stored value.
     * 
     * @param newValue      Sets the slider position and stored value. Values
     *                       will be restricted to the current slider range.
     * 
     * @param notification  Determines if listeners will be notified of
     *                       this change.
     */
    void setValue(double newValue,
            NotificationType notification = sendNotificationAsync);

    /**
     * @return the current slider position value.
     */
    double getValue() const;

    /**
     * @param listener   This will receive updates whenever the slider value
     *                    changes.
     */
    void addListener(Slider::Listener* listener);

    /**
     * Use this to determine which IconSliderComponent is responsible for
     * a slider callback.
     * 
     * @param sliderPtr  A pointer to any slider object.
     * 
     * @return           true iff slider belongs to this IconSliderComponent.
     */
    bool ownsSlider(Slider * sliderPtr);

    /** Sets the limits that the slider's value can take.
     * 
     * @param newMinimum  The lowest permitted value.
     * 
     * @param newMaximum  The highest permitted value.
     * 
     * @param newInterval The steps in which the value is allowed to increase. 
     *                     If this is not zero, the value will always be 
     *                     (newMinimum + (newInterval * an integer)).
     */
    void setRange(
            double newMinimum,
            double newMaximum,
            double newInterval = 0);
    
private:
    /**
     * Update the icons and slider to fit the component bounds.
     */
    void resized() override;
    
    /**
     * Update the icon color if slider color changes
     */
    void colourChanged() override;


    //Icon displayed to the left of the slider.
    DrawableImageComponent lowIcon;

    //Icon displayed to the right of the slider.
    DrawableImageComponent highIcon;

    //Internal slider component.
    Slider slider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconSliderComponent)
};
