/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppMenu.cpp
 * Author: anthony
 * 
 * Created on December 19, 2017, 1:37 PM
 */

#include "AppMenu.h"
#include <sstream>
#include <functional>

AppMenu::AppMenu() {
    int menu_width = 0;
    int menu_height = 0;
    std::function<void(AppMenuButton*)> addButton=[this,&menu_width,&menu_height](AppMenuButton* appButton){ 
        std::stringstream index;
        index << this->launchButtons.size();
        int w = appButton->getWidth();
        int h = appButton->getHeight();
        if (this->launchButtons.size() == 0)menu_width = w;
        menu_height += h;
        int x = 0;
        int y = h*this->launchButtons.size();
        appButton->setComponentID(index.str());
        appButton->setBounds(x, y, w, h);
        addAndMakeVisible(appButton);
        appButton->setEnabled(true);
        appButton->setVisible(true);
        appButton->addListener(this);
        this->nameMap[appButton->getName()]=appButton;
        this->launchButtons.push_back(appButton);
    };
    std::vector<String> categories= desktopEntries.getCategoryNames();
    for (int i = 0;i < categories.size();i++){
        addButton(new AppMenuButton(DesktopEntry(categories[i])));
        
    }
    for (int i = 0; i < desktopEntries.size(); i++) {
        DesktopEntry d = desktopEntries.getEntry(i);
        if (d.hidden() || d.noDisplay())continue;
        std::stringstream index;
        index << launchButtons.size();
        AppMenuButton * dButton = new AppMenuButton(d);
        int w = dButton->getWidth();
        int h = dButton->getHeight();
        if (launchButtons.size() == 0)menu_width = w;
        menu_height += h;
        int x = 0;
        int y = h*launchButtons.size();
        dButton->setComponentID(index.str());
        dButton->setBounds(x, y, w, h);
        addAndMakeVisible(dButton);
        dButton->setEnabled(true);
        dButton->setVisible(true);
        dButton->addListener(this);
        nameMap[dButton->getName()]=dButton;
        launchButtons.push_back(dButton);
    }

    setSize(menu_width, menu_height);
    std::cout << "added " << launchButtons.size() << " buttons\n";
}

AppMenu::~AppMenu() {
    for (int i = launchButtons.size(); i >= 0; i++){
        AppMenuButton * toDelete = launchButtons.back();
        launchButtons.pop_back();
        delete toDelete;
    }
    selected = NULL;
}



void AppMenu::buttonClicked(Button* buttonClicked) {
    if (selected != NULL) {
        selected->setSelected(false);
        selected->repaint();
    }
    selected = (AppMenuButton *) buttonClicked;
    selected->setSelected(true);
    selected->repaint();

    //move AppMenu to center the selected button, if it's not near an edge
    int buttonPos = selected->getY();
    int screenHeight = Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight();
    Rectangle<int> dest(getBounds());
    dest.setY(-buttonPos + screenHeight / 2);
    if (dest.getY() > 0) {
        dest.setY(0);
    } else if (getHeight() > screenHeight && dest.getBottom() < screenHeight) {
        dest.setBottom(screenHeight);
    }
    Desktop::getInstance().getAnimator().animateComponent(this, dest, getAlpha(), 100, true, 1, 1);
}


void AppMenu::selectIndex(int index){
    if(index < launchButtons.size() 
            && index >= 0)launchButtons[index]->triggerClick();
    
}
int AppMenu::getSelectedIndex(){
    if(selected==NULL)return -1;
    return selected->getComponentID().getIntValue();
}

void AppMenu::selectNext() {
    if (selected == NULL)selectIndex(0);
    else selectIndex(getSelectedIndex()+1);
}

void AppMenu::selectPrevious() {
    if (selected == NULL)selectIndex(0);
    else selectIndex(getSelectedIndex()-1);
}



