/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppMenu.h
 * Author: anthony
 *
 * Created on December 19, 2017, 1:37 PM
 */

#ifndef APPMENU_H
#define APPMENU_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "AppMenuButton.h"
#include "DesktopEntries.h"

class AppMenu : public Component,
public Button::Listener{
public:
    AppMenu();
    virtual ~AppMenu();
    void buttonClicked (Button* buttonClicked) override;
    void selectNext();
    void selectPrevious();
private:
    DesktopEntries desktopEntries;
    void selectIndex(int index);
    int getSelectedIndex();
    std::vector<AppMenuButton*> launchButtons;
    std::map<String,AppMenuButton*> nameMap;
    AppMenuButton* selected=NULL;
};

#endif /* APPMENU_H */

