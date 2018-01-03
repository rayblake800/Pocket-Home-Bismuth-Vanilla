#ifndef INPUTPAGE_H
#define INPUTPAGE_H

#include "../Utils.h"
#include "../PokeLookAndFeel.h"

using namespace juce;


class InputSettingsPage: public Component, public Button::Listener, private ComboBox::Listener{
public:
  InputSettingsPage();
  ~InputSettingsPage();
  void buttonClicked(Button*) override;
  void paint(Graphics&) override;
  void resized() override;
  void comboBoxChanged(ComboBox*) override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSettingsPage)
  //BackButton
  ScopedPointer<ImageButton> backButton;
  //Background color
  Colour bg_color;
  //Title of the pane
  Label title;
  //Combobox
  Label cursorvisible;
  ComboBox choosemode;
  
  //Button for calibrating
  TextButton calibrating;
  //Button for the FN key (xmodmap)
  TextButton fnmapping;
};

#endif