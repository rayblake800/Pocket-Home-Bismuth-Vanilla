/**
 * @file LoginPage.h
 * 
 * TODO: Documentation
 */
#include "JuceHeader.h"
#include "ScalingLabel.h"
#include "DrawableImageComponent.h"
#include "PageComponent.h"
#pragma once
class LoginPage : public PageComponent, public TextEditor::Listener{
public:
  LoginPage(ComponentConfigFile& config,std::function<void ()> loginCallback);
  
  ~LoginPage() { }

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
