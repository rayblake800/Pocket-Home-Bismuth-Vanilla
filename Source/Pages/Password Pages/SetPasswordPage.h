/**
 * @file SetPasswordPage.h
 * 
 * Allows the user to set or change a password for this application.  Honestly,
 * this isn't really much of a security measure, but it's enough to keep the
 * average person from using this particular application.
 */
#pragma once
#include "ScalingLabel.h"
#include "PageComponent.h"
#include "JuceHeader.h"

class SetPasswordPage : public PageComponent{
public:
  SetPasswordPage(ComponentConfigFile& config);
  
  virtual ~SetPasswordPage() { }
  
private:
    
  /**
   * If the setPassword button is clicked, attempts to set a new application
   * password.  The result of this operation will be displayed in a message box,
   * and all text fields on the page will be cleared.  If the password was
   * set successfully, the page will be closed.
   * 
   * @param button  Should only ever be the setPassword button.
   */
  void pageButtonClicked(Button* button) override;
  
  /**
   * Opens a message box to display an error message, and clears all text entry
   * fields on the page.
   * 
   * @param title  The title to print on the message box.
   * 
   * @param error  The error message to print in the message box.
   */
  void showErrorMessage(String title,String error);
  
  /**
   * Clears the text in all text entry fields on the page.
   */
  void clearAllFields();
  
  //Page title
  ScalingLabel title;
  
  //Text field for the root password
  ScalingLabel rootLabel;
  TextEditor rootPassword;
  
  //Text field for the current password (if there is one)
  ScalingLabel curLabel;
  TextEditor curPassword;
  
  //Text field for the new password
  ScalingLabel newLabel;
  TextEditor newPassword;
  
  //Text field for the confirmation of the new password
  ScalingLabel confirmLabel;
  TextEditor confirmPassword;
  
  //Button for applying the settings
  TextButton setPassword;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SetPasswordPage)
};
