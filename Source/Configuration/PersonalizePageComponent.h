#pragma once

#include "../Basic Components/FileSelectTextEditor.h"
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
  Image bgImage;
  
  /* Labels for inputs */
  Label bgTitle;
  Label iconTitle;
  
  /* Labels to show before the inputs */
  Label bgLabel;
  Label nameLabel;
  Label iconLabel;
  Label shellLabel;
  Label successLabel;
  
  /* Inputs */
  ComboBox bgTypePicker;
  TextButton addFaveAppBtn;
  FileSelectTextEditor bgEditor; 
  
  TextEditor appNameEditor;
  FileSelectTextEditor appIconEditor;
  TextEditor launchCmdEditor;
  
  TextButton applyChangesBtn;
};