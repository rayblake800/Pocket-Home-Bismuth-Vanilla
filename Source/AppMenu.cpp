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
    setSize (600, 400);
    numButtons = 0;
    for (int i = 0; i < de.size(); i++) {
        if (!de.getEntry(i).hidden()&&!de.getEntry(i).noDisplay())numButtons++;
    }
    launchButtons = new ScopedPointer<AppMenuButton>[numButtons];
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
        int x = 0;
        int y = ((h+2)*arrayIndex);
        launchButtons[arrayIndex]->setComponentID(index.str());
        launchButtons[arrayIndex]->setBounds(x, y, w, h);
        addAndMakeVisible(launchButtons[arrayIndex]);
        launchButtons[arrayIndex]->setEnabled(true);
        launchButtons[arrayIndex]->setVisible(true);
        launchButtons[arrayIndex]->addListener(this);
        arrayIndex++;
    }
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
    if(selected!=NULL)selected->setSelected(false);
    selected->repaint();
    selected = (AppMenuButton *) buttonClicked;
    selected->setSelected(true);
    selected->repaint();
}

