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
    void openFolder(String categoryName);
private:
    DesktopEntries desktopEntries;
    void selectIndex(int index);
    std::map<String,AppMenuButton*> nameMap;

    std::vector<std::vector<AppMenuButton*>> buttonColumns;
    std::vector<AppMenuButton*> selected;
};

#endif /* APPMENU_H */

