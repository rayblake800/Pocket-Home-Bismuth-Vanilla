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
    numButtons = 0;
    for (int i = 0; i < de.size(); i++) {
        if (!de.getEntry(i).hidden()&&!de.getEntry(i).noDisplay())numButtons++;
    }
    launchButtons = new ScopedPointer<AppMenuButton>[numButtons];
    int menu_width=0;
    int menu_height=0;
    int arrayIndex = 0;
    for (int i = 0; i < de.size(); i++) {
        if (arrayIndex >= numButtons) {
            std::cout << "array index exceeds button count!\n";
            break;
        }
        DesktopEntry d = de.getEntry(i);
        if (d.hidden() || d.noDisplay())continue;
        std::stringstream index;
        index << i;
        launchButtons[arrayIndex] = new AppMenuButton(d);
        int w = launchButtons[arrayIndex]->getWidth();
        int h = launchButtons[arrayIndex]->getHeight();
        if(arrayIndex==0)menu_width=w;
        menu_height+=h;
        int x = 0;
        int y =h*arrayIndex;
        launchButtons[arrayIndex]->setComponentID(index.str());
        launchButtons[arrayIndex]->setBounds(x, y, w, h);
        addAndMakeVisible(launchButtons[arrayIndex]);
        launchButtons[arrayIndex]->setEnabled(true);
        launchButtons[arrayIndex]->setVisible(true);
        launchButtons[arrayIndex]->addListener(this);
        arrayIndex++;
    }
    
    setSize (menu_width,menu_height);
    std::cout << "added " << numButtons << " buttons\n";
}
AppMenu::~AppMenu() {
    for (int i = 0; i < numButtons; i++)delete launchButtons[i];
    delete[] launchButtons;
    launchButtons=NULL;
    selected=nullptr;
}

void AppMenu::buttonClicked (Button* buttonClicked)
{
    if(selected!=NULL){
        selected->setSelected(false);
        selected->repaint();
    }
    selected = (AppMenuButton *) buttonClicked;
    selected->setSelected(true);
    selected->repaint();
    
    //move AppMenu to center the selected button, if it's not near an edge
    int buttonPos=selected->getY();
    int screenHeight=Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight();
    Rectangle<int> dest(getBounds());
    dest.setY(-buttonPos+screenHeight/2);
    if(dest.getY()>0){
        dest.setY(0);
    }
    else if(getHeight()>screenHeight && dest.getBottom()<screenHeight){
        dest.setBottom(screenHeight);
    }
    Desktop::getInstance().getAnimator().animateComponent(this,dest,getAlpha(),100,true,1,1);
    //buttonPos-x=mid
    //x=buttonPos-mid
    
    
    
}

