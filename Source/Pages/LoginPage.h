/**
 * @file LoginPage.h
 * 
 * TODO: Documentation
 */
#include "../Basic Components/ScalingLabel.h"
#include "../Basic Components/DrawableImageComponent.h"
#include "../PageComponent.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#pragma once
class LoginPage : public PageComponent, public TextEditor::Listener{
public:
  LoginPage(std::function<void ()> loginCallback);
  ~LoginPage();

  virtual bool hasPassword();
  virtual void textFocus();
  
private:
  virtual void pageButtonClicked(Button *button) override;
  virtual void textEditorReturnKeyPressed(TextEditor&) override;
  void checkForPassword();
  void displayError();
  
  std::function<void()> loginCallback;
  bool foundPassword;
  TextButton loginButton;
  ScalingLabel passwordLabel;
  TextEditor passwordField;
  String hashedPassword;
  DrawableImageComponent ntcIcon;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoginPage)
};
