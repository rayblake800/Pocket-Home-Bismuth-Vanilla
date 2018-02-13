/**
 * @file LoginPage.h
 * 
 * TODO: Documentation, layout management, organization
 */
#include "../JuceLibraryCode/JuceHeader.h"
#pragma once
class LoginPage : public Component, public Button::Listener, public TextEditor::Listener{
public:
  LoginPage(std::function<void ()> loginCallback);
  ~LoginPage();
  
  PageStackComponent * getPageStack();
  
  virtual void resized() override;
  virtual void paint(Graphics &) override;
  virtual void buttonClicked(Button *button) override;
  virtual bool hasPassword();
  virtual void textFocus();
  virtual void textEditorReturnKeyPressed(TextEditor&) override;
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoginPage)
  void displayError();
  
  std::function<void()> loginCallback;
  bool foundPassword;
  ScopedPointer<TextButton> log;
  ScopedPointer<Label> label_password;
  String hashed_password;
  ScopedPointer<DrawableImage> bgImage;
  ScopedPointer<DrawableImage> ntcIcon;
  ScopedPointer<TextEditor> cur_password;
};
