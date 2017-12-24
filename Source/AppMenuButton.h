/* 
 * File:   AppMenuButton.h
 * Author: anthony
 * 
 * Created on December 17, 2017, 3:57 PM
 * Defines a custom button type for applications and application directories
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DesktopEntry.h"
class AppMenuButton : public TextButton {
public:
  //button size definitions:
  //consider putting these in a config file
  /**
   * Create a new button representing an application or folder
   * @param desktopEntry defines the application/directory data
   * @param index button position in its column
   * @param column button's column in the AppMenu
   * @param width measured in pixels
   * @param height measured in pixels
   */
  AppMenuButton(DesktopEntry desktopEntry,int index,int column,int width,int height);
  
  /**
   * Set whether this button is currently selected.
   * @param select new selection state
   */
  void setSelected(bool select);
  
  /**
   * @return true if this button is for an application folder
   */
  bool isFolder();
  
  /**
   * @return the display name of the associated application
   */
  String getAppName();
  
  /**
   * @return application shell command or directory path.
   */
  String getCommand();
  
  /**
   * @return button position in its column
   */
  int getIndex();
  
  /**
   * @return button's column in the AppMenu
   */
  int getColumn();
  
  /**
   * If called, the button will not be drawn outside of clipRegion
   * @param clipRegion a screen coordinate rectangle
   */
  void setClipRegion(Rectangle<int>clipRegion);
  
    //custom button draw routines
  void paint (Graphics& g) override;
  private:
      DesktopEntry desktopEntry;
      bool selected=false;
      Image appIcon;
      //index in the column, starting at the top
      int index;
      //containing column number, counted left to right
      int column;
      Colour fillColour = Colours::transparentWhite;
      Colour selectedFillColour = Colour (0xffeb008b);
      bool clip=false;
      Rectangle<int> clipRegion;
};
