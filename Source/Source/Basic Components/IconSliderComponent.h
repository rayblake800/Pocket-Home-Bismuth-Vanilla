#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class IconSliderComponent : public Slider{
public:
  ScopedPointer<Drawable> lowIcon;
  ScopedPointer<Drawable> highIcon;
  StretchableLayoutManager sliderLayout;

  IconSliderComponent(const Drawable& low, const Drawable& high);
  ~IconSliderComponent();

  void resized() override;
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconSliderComponent)
};
