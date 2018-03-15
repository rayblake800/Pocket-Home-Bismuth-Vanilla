/**
 * @file RemovePasswordPage.h
 * 
 * TODO: documentation, relative layout management
 */
#pragma once
#include "JuceHeader.h"
#include "ScalingLabel.h"
#include "PageComponent.h"

class RemovePasswordPage : public PageComponent{
public:
  RemovePasswordPage();
  ~RemovePasswordPage();
  static String hashString(const String&);
  bool hasPassword();
  
private:
  void pageButtonClicked(Button*) override;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RemovePasswordPage)
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
  void deletePassword();
  
  //Text field for the root password
  ScalingLabel root_label;
  TextEditor root_password;
  //Text field for the current password (if there is)
  ScalingLabel cur_label;
  TextEditor cur_password;
  //Button for applying the settings
  TextButton apply;
};
