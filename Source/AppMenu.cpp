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
    selected.push_back(NULL);
    buttonColumns.emplace(buttonColumns.begin());
    int menu_width = 0;
    int menu_height = 0;
    std::function<void(AppMenuButton*)> addButton=[this,&menu_width,&menu_height](AppMenuButton* appButton){ 
        std::stringstream index;
        index << this->buttonColumns[0].size();
        int w = appButton->getWidth();
        int h = appButton->getHeight();
        if (this->buttonColumns[0].size() == 0)menu_width = w;
        menu_height += h;
        int x = 0;
        int y = h*this->buttonColumns[0].size();
        appButton->setComponentID(index.str());
        appButton->setBounds(x, y, w, h);
        addAndMakeVisible(appButton);
        appButton->setEnabled(true);
        appButton->setVisible(true);
        appButton->addListener(this);
        this->nameMap[appButton->getName()]=appButton;
        this->buttonColumns[0].push_back(appButton);
    };
    std::vector<String> categories= desktopEntries.getCategoryNames();
    for (int i = 0;i < categories.size();i++){
        addButton(new AppMenuButton(DesktopEntry(categories[i]),buttonColumns[0].size(),0));        
    }
    for (int i = 0; i < desktopEntries.size(); i++) {
        DesktopEntry d = desktopEntries.getEntry(i);
        addButton(new AppMenuButton(d,buttonColumns[0].size(),0));
    }
    setSize(menu_width, menu_height);
    std::cout << "added " << buttonColumns[0].size() << " buttons\n";
}

AppMenu::~AppMenu() {
    for (int i = buttonColumns[0].size(); i >= 0; i++){
        AppMenuButton * toDelete = buttonColumns[0].back();
        buttonColumns[0].pop_back();
        delete toDelete;
    }
    selected[0] = NULL;
}



void AppMenu::buttonClicked(Button* buttonClicked) {
    if (selected[0] != NULL) {
        selected[0]->setSelected(false);
        selected[0]->repaint();
    }
    selected[0] = (AppMenuButton *) buttonClicked;
    selected[0]->setSelected(true);
    selected[0]->repaint();

    //move AppMenu to center the selected button, if it's not near an edge
    int buttonPos = selected[0]->getY();
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
    if(index < buttonColumns[0].size() 
            && index >= 0)buttonColumns[0][index]->triggerClick();
    
}

void AppMenu::selectNext() {
    if (selected[0] == NULL)selectIndex(0);
    else selectIndex(selected[0]->getIndex()+1);
}

void AppMenu::selectPrevious() {
    if (selected[0] == NULL)selectIndex(0);
    else selectIndex(selected[0]->getIndex()-1);
}



