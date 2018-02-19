#pragma once

#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "../Basic Components/FileSelectTextEditor.h"
#include "../Basic Components/ScalingLabel.h"
#include "../PageComponent.h"
#include "../Utils.h"

class PersonalizePage : public PageComponent, private ComboBox::Listener
, private FileSelectTextEditor::Listener {
public:
  PersonalizePage();
  ~PersonalizePage();
  
  
private:
  void comboBoxChanged(ComboBox*) override;
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
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PersonalizePage)
};