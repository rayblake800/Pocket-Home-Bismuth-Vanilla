#pragma once

#include "ConfigurableImageButton.h"
#include "FileSelectTextEditor.h"
#include "ScalingLabel.h"
#include "CounterComponent.h"
#include "PageComponent.h"
#include "Utils.h"
#include "ColourPage.h"

class HomeSettingsPage : public PageComponent, private ComboBox::Listener
, private FileSelectTextEditor::Listener {
public:
  HomeSettingsPage();
  ~HomeSettingsPage();
private:
  /**
   * Update AppMenu dimensions when the page closes.
   */
  void pageRemovedFromStack() override;

  void comboBoxChanged(ComboBox*) override;
  
  void pageButtonClicked(Button* button) override;

  
  void fileSelected(FileSelectTextEditor* edited);
  
  void updateComboBox();
  ScalingLabel title;
  Colour bgColor;
  ScalingLabel bgTitle;
  ScalingLabel bgLabel;
  ComboBox bgTypePicker;
  FileSelectTextEditor bgEditor;
  
  ScalingLabel menuPickerLabel;
  ComboBox menuTypePicker;
  
  ScalingLabel columnCountLabel;
  CounterComponent columnCounter;
  
  ScalingLabel rowCountLabel;
  CounterComponent rowCounter;
  TextButton colourPageBtn;
  ColourPage colourPage;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeSettingsPage)
};
