#pragma once

#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "../Basic Components/FileSelectTextEditor.h"
#include "../Basic Components/ScalingLabel.h"
#include "../GridLayoutManager.h"
#include "../Utils.h"

class PersonalizePageComponent : public Component, private Button::Listener, 
        private ComboBox::Listener, private FileSelectTextEditor::Listener {
public:
  PersonalizePageComponent();
  ~PersonalizePageComponent();
  
  void paint(Graphics &g) override;
  void resized() override;
  void buttonClicked(Button*) override;
  void comboBoxChanged(ComboBox*) override;
  void fileSelected(FileSelectTextEditor* edited);
  
private:
  void updateComboBox();
  GridLayoutManager layoutManager;
  ConfigurableImageButton backButton;
  Colour bgColor;
  ScalingLabel bgTitle;
  ScalingLabel bgLabel;
  ComboBox bgTypePicker;
  FileSelectTextEditor bgEditor;
  
  ScalingLabel menuPickerLabel;
  ComboBox menuTypePicker;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PersonalizePageComponent)
};