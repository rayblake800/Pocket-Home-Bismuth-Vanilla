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
#include "DesktopEntries.h"
#include <sstream>

AppMenu::AppMenu() {
    DesktopEntries de;
    int menu_width = 0;
    int menu_height = 0;
    for (int i = 0; i < de.size(); i++) {
        DesktopEntry d = de.getEntry(i);
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



