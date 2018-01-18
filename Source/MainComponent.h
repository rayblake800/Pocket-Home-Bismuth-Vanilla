#pragma once
#include "Settings/SettingsPageComponent.h"
#include "Power/PowerPageComponent.h"
#include "AppMenu Page/AppMenuPage.h"
#include "PokeLookAndFeel.h"
#include "LoginPage.h"
#include "PageStackComponent.h"

class MainContentComponent;


class MainContentComponent : public Component {
public:

  MainContentComponent();
  ~MainContentComponent();

  void paint(Graphics &) override;
  void resized() override;
  void loggedIn();
  PageStackComponent& getPageStack();
  
  void handleMainWindowInactive();

private:
  AppMenuPage appMenuPage;
  PokeLookAndFeel lookAndFeel;
  PageStackComponent pageStack;
  LoginPage loginPage;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
