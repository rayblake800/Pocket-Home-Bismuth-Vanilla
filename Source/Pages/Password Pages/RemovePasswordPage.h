/**
 * @file RemovePasswordPage.h
 * 
 * This page allows the user to remove the Pocket-Home password.  To do so, the
 * user must enter in the existing Pocket-Home password, along with their root
 * password.
 */
#pragma once
#include "JuceHeader.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

class RemovePasswordPage : public PageComponent{
public: 
  RemovePasswordPage(ComponentConfigFile& config);
  
  ~RemovePasswordPage() {}
  
private:
    
  /**
   * Attempts to delete the Pocket-Home password when deleteButton is pressed.
   * If this succeeds, the page will close after showing an AlertWindow.
   * 
   * @param button  This should always be deleteButton
   */
  void pageButtonClicked(Button* button) override;
  
  /**
   * Clears all TextEditor fields on the page.
   */
  void clearAllFields();
  
  //Title of the page
  ScalingLabel titleLabel;
  
  //Text field for the root password
  ScalingLabel rootLabel;
  TextEditor rootPassword;
  
  //Text field for the current password
  ScalingLabel curPwdLabel;
  TextEditor curPassword;
  
  //Triggers the attempted password removal
  TextButton deleteButton;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RemovePasswordPage)
};
