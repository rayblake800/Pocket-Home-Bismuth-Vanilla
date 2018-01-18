#pragma once

#include "../Basic Components/FileSelectTextEditor.h"
#include "../Basic Components/ScalingLabel.h"
#include "../Configuration/AppConfigFile.h"
#include "../Utils.h"

class PersonalizePageComponent : public Component, private Button::Listener, private ComboBox::Listener {
public:
  PersonalizePageComponent(AppConfigFile& appConfig);
  ~PersonalizePageComponent();
  
  void paint(Graphics &g) override;
  void resized() override;
  void buttonClicked(Button*) override;
  void comboBoxChanged(ComboBox*) override;
  void showAddComponents(bool);
  void resetApplySuccess();
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PersonalizePageComponent)
  AppConfigFile& appConfig;
  bool updateJSON();
  void updateComboBox();
  
  ScopedPointer<ImageButton> backButton;
  Colour bgColor;
  
  /* Labels for inputs */
  ScalingLabel bgTitle;
  ScalingLabel iconTitle;
  
  /* Labels to show before the inputs */
  ScalingLabel bgLabel;
  ScalingLabel nameLabel;
  ScalingLabel iconLabel;
  ScalingLabel shellLabel;
  ScalingLabel successLabel;
  
  /* Inputs */
  ComboBox bgTypePicker;
  TextButton addFaveAppBtn;
  FileSelectTextEditor bgEditor; 
  
  TextEditor appNameEditor;
  FileSelectTextEditor appIconEditor;
  TextEditor launchCmdEditor;
  
  TextButton applyChangesBtn;
};