/**
 * @file SettingsCategoryButton.h
 * 
 * TODO: documentation
 */
#include "../../../JuceLibraryCode/JuceHeader.h"
#pragma once
class SettingsCategoryButton : public Button {
public:

  SettingsCategoryButton(const String &name);
  ~SettingsCategoryButton() {}

  void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown) override;
  void resized() override;

  void setText(const String &text);

private:
  String displayText;
  Rectangle<int> pillBounds;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsCategoryButton)
};