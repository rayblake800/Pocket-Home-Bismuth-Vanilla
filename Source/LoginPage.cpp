/*
  ==============================================================================

    LoginPage.cpp
    Created: 2 Jan 2018 2:41:17am
    Author:  anthony

  ==============================================================================
*/
#include "PocketHomeApplication.h"
#include "Settings/LoginSettingsPage.h"
#include "LoginPage.h"

LoginPage::LoginPage(std::function<void ()> loginCallback):
bgImage(new DrawableImage),
ntcIcon(new DrawableImage),
cur_password(new TextEditor("field_password", 0x2022)),
label_password(new Label("pass", "Password :")),
log(new TextButton("login")), hashed_password("none"),
loginCallback(loginCallback), foundPassword(false)
{
  this->setBounds(0, 0, 480, 272);
  Image bg = createImageFromFile(assetFile("login/background.png"));
  bgImage->setImage(bg);
  Image ntc = createImageFromFile(assetFile("login/ntcbanner.png"));
  ntcIcon->setImage(ntc);
  log->setButtonText("Log In");
  
  label_password->setFont(Font(14.f, Font::FontStyleFlags::bold));
  label_password->setBounds(140, 160, 100, 30);
  cur_password->setBounds(140, 190, 200, 30);
  log->setBounds(140, 230, 200, 30);
  log->addListener(this);
  
  this->hasPassword();
  /* Adding a listener for catching the moment return key is pressed */
  cur_password->addListener(this);
  
  addAndMakeVisible(bgImage, 1);
  addAndMakeVisible(ntcIcon, 4);
  addAndMakeVisible(cur_password, 3);
  addAndMakeVisible(label_password, 2);
  
  if(!foundPassword){
      cur_password->setVisible(false);
      label_password->setVisible(false);
  }
  addAndMakeVisible(log, 3);
}

LoginPage::~LoginPage(){}

bool LoginPage::hasPassword(){
  label_password->setVisible(false);
  cur_password->setVisible(false);
  char* home_str = getenv("HOME");
  String home(home_str);
  File passwd(home+"/.pocket-home/.passwd/passwd");
  if(passwd.exists()){
    String content = passwd.loadFileAsString();
    content = content.removeCharacters("\n");
    if(content==String("none")) return false;
    else hashed_password = content;
    foundPassword = true;
    label_password->setVisible(true);
    cur_password->setVisible(true);
    return true;
  }
  return false;
}



void LoginPage::resized() {
  auto bounds = getLocalBounds();
  ntcIcon->setBounds(0, 0, 480, 130);
  bgImage->setBounds(0, 0, bgImage->getWidth(), bgImage->getHeight());
}

void LoginPage::displayError(){
  AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon,
                "Wrong password", 
                "Wrong password, try again",
                "Ok");
}

void LoginPage::textFocus(){
  cur_password->grabKeyboardFocus();
  cur_password->setWantsKeyboardFocus(true);
}

void LoginPage::buttonClicked(Button *button){
  String pass_tmp = cur_password->getText();
  String hashed = LoginSettingsPage::hashString(pass_tmp);
  if(hashed_password=="none" || hashed == hashed_password){
      cur_password->setText("");
      loginCallback();
  }
  else displayError();
}

void LoginPage::textEditorReturnKeyPressed(TextEditor& te){
  /* log is a ScopedPointer<TextButton>, so *log, will have type Button
   * and so &(*log) has type Button*, required for buttonClicked
   */
  buttonClicked(&(*log));
}

void LoginPage::paint(Graphics &g) {
  //g.fillAll(Colours::white);
}
