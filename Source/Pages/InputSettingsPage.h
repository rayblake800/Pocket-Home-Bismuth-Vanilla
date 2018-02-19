/**
 * @file InputSettingsPage.h
 * 
 * TODO: documentation, cleanup.
 */
#pragma once
#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "../PageComponent.h"
class InputSettingsPage: public PageComponent, private ComboBox::Listener{
public:
  InputSettingsPage();
  ~InputSettingsPage();
  void pageButtonClicked(Button*) override;
  void comboBoxChanged(ComboBox*) override;

private:
    //Title of the pane
    ScalingLabel title;
    //Combobox
    ScalingLabel cursorVisible;
    ComboBox choosemode;

    //Button for calibrating
    TextButton calibrating;
    //Button for the FN key (xmodmap)
    TextButton fnmapping;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSettingsPage)
};
