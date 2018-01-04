#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class IconSliderComponent : public Component{
public:
  IconSliderComponent(Drawable * low,Drawable * high);
  ~IconSliderComponent();
  
  void setValue(double newValue, 
          NotificationType notification=sendNotificationAsync);
  double getValue() const;
  
  void addListener(Slider::Listener* listener);
  
  /**
   * Use this to determine which IconSliderComponent is responsible for
   * a slider callback.
   * @param sliderPtr a pointer to a slider object
   * @return true iff slider belongs to this IconSliderComponent
   */
  bool ownsSlider(Slider * sliderPtr);
private:
  void resized() override;
  ScopedPointer<DrawableButton> lowIcon;
  ScopedPointer<DrawableButton> highIcon;
  StretchableLayoutManager sliderLayout;
  Slider slider;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconSliderComponent)
};
