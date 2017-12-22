/*
  ==============================================================================

    AppMenuButton.h
    Created: 17 Dec 2017 3:57:53pm
    Author:  anthony

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DesktopEntry.h"
class AppMenuButton : public TextButton {
public:
  AppMenuButton(DesktopEntry desktopEntry,int index,int column);
  void paint (Graphics& g) override;
  
  void setSelected(bool select);
  bool isFolder();
  //Get the display name of the associated application
  String getAppName();
  String getCommand();
  int getIndex();
  int getColumn();
  
  private:
      DesktopEntry dEntry;
      bool folder=false;
      bool selected=false;
      Image appIcon;
      //index in the column, starting at the top
      int index;
      //containing column number, counted left to right
      int column;
      Colour fillColour = Colour (0xff939089);
      Colour selectedFillColour = Colour (0xffffade8);
};
