/**
 * @file InputSettingsPage.h
 * 
 * TODO: documentation, cleanup.
 */
#pragma once
#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "../GridLayoutManager.h"
class InputSettingsPage: public Component, public Button::Listener, private ComboBox::Listener{
public:
  InputSettingsPage();
  ~InputSettingsPage();
  void buttonClicked(Button*) override;
  void paint(Graphics&) override;
  void resized() override;
  void comboBoxChanged(ComboBox*) override;

private:
    GridLayoutManager layoutManager;
    //BackButton
    ConfigurableImageButton backButton;
    //Background color
    Colour bg_color;
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
