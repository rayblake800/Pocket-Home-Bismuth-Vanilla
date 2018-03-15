/**
 * @file SetPasswordPage.h
 * 
 * TODO: documentation, relative layout management
 */
#pragma once
#include "ScalingLabel.h"
#include "PageComponent.h"
#include "JuceHeader.h"

class SetPasswordPage : public PageComponent{
public:
  SetPasswordPage();
  ~SetPasswordPage();
  
  void pageButtonClicked(Button*) override;
  static String hashString(const String&);
  bool hasPassword();
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SetPasswordPage)
  //Title of the pane
  ScalingLabel title;
  //Does the home have a password ?
  bool has_password;
  bool has_file;
  //Current hashed password
  String cur_hashed;
  
  void clearAllFields();
  void loadPassword();
  bool isPasswordCorrect(const String&);
  void savePassword();
  void createIfNotExists();
  bool passwordIdentical();
  
  //Text field for the root password
  ScalingLabel root_label;
  TextEditor root_password;
  //Text field for the current password (if there is)
  ScalingLabel cur_label;
  TextEditor cur_password;
  //Text field for the new password
  ScalingLabel new_label;
  TextEditor new_password;
  //Text field for the confirmation of the new password
  ScalingLabel ret_label;
  TextEditor ret_password;
  //Button for applying the settings
  TextButton apply;
};
