#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Grid.h"
#include "AppMenu.h"
#include "AppMenuButton.h"

class AppsPageComponent;
class LauncherComponent;

class AppCheckTimer : public Timer {
public:
  AppCheckTimer() {};
  virtual void timerCallback() override;
  // TODO: better pointer usage, weakref for cycle relationship?
  AppsPageComponent* appsPage;
};

class AppDebounceTimer : public Timer {
public:
  AppDebounceTimer() {};
  virtual void timerCallback() override;
  // TODO: better pointer usage, weakref for cycle relationship?
  AppsPageComponent* appsPage;
};

class AppListComponent : 
public Component, public Button::Listener{
public:
  AppListComponent(Component* parent = nullptr, bool = true);
  ~AppListComponent();
  ScopedPointer<AppMenu> appMenu;
  
  void resized() override;
  void buttonStateChanged(Button*) override {};
  void buttonClicked(Button *button) override {};
  
  void mouseDrag(const MouseEvent&) override {};
  // FIXME: this is barsize from launcher component
  double btnHeight = 50;
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppListComponent)
};


class AppsPageComponent : public AppListComponent{
public:
  AppsPageComponent(LauncherComponent* launcherComponent, bool);
  ~AppsPageComponent();
  
  
  OwnedArray<ChildProcess> runningApps;
  
  //void buttonClicked(Button *button) override;
  bool keyPressed (const KeyPress &) override;
  
  void checkRunningApps();
  
  bool debounce = false;

private:
  using AppRunningMap = HashMap<AppMenuButton*, int>;
  ScopedPointer<Drawable> cpy;

  DrawableButton* appsLibraryBtn;
  LauncherComponent* launcherComponent;
  
  AppRunningMap runningAppsByButton;
  AppCheckTimer runningCheckTimer;
  AppDebounceTimer debounceTimer;

  void onTrash(Button*);
  void startApp(AppMenuButton * appButton);
  void focusApp(AppMenuButton* appButton, const String& windowId);
  void startOrFocusApp(AppMenuButton* appButton);
  void moveInConfig(AppMenuButton*, bool);
  
  //Trash Icon
  ScopedPointer<ImageButton> trashButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppsPageComponent)
  
  //Store mouse dragged event X and Y
  int x;
  int y;
  bool shouldMove;
};
