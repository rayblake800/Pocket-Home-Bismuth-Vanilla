/**
 * @file InputSettingsPage.h
 * 
 * TODO: documentation, cleanup.
 */
#pragma once
#include "ConfigurableImageButton.h"
#include "ScalingLabel.h"
#include "PageComponent.h"
class InputSettingsPage: public PageComponent, private ComboBox::Listener{
public:
  InputSettingsPage(PageFactoryInterface* pageFactory,
          MainConfigFile& mainConfig, 
          ComponentConfigFile& componentConfig);
  
  virtual ~InputSettingsPage() { }
  void pageButtonClicked(Button*) override;
  void comboBoxChanged(ComboBox*) override;

private:
    MainConfigFile& mainConfig;
    
    //Title of the pane
    ScalingLabel title;
    //Combobox
    ScalingLabel cursorVisible;
    ComboBox chooseMode;
   
    //Button for calibrating
    TextButton calibrating;
    //Button for the FN key (xmodmap)
    TextButton fnmapping;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSettingsPage)
};
