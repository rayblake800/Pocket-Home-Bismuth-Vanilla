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
    std::vector<String> categories = desktopEntries.getMainCategories(true);
    for (int i = 0; i < categories.size(); i++) {
        addButton(new AppMenuButton(DesktopEntry(categories[i]), buttonColumns[activeColumn()].size(), activeColumn()));
    }
    /**
    for (int i = 0; i < desktopEntries.size(); i++) {
        DesktopEntry d = desktopEntries.getEntry(i);
        addButton(new AppMenuButton(d,buttonColumns[0].size(),0));
    }
     **/
    std::cout << "added " << buttonColumns[activeColumn()].size() << " buttons\n";
}

AppMenu::~AppMenu() {
    while (!buttonColumns.empty())closeFolder();
}

void AppMenu::openFolder(String categoryName) {
    std::vector<DesktopEntry*> folderItems = desktopEntries.getCategoryEntries(categoryName);
    if (folderItems.empty())return;
    selected.push_back(NULL);
    int columnWidth = 0;
    for (int i = 0; i < folderItems.size(); i++) {
        DesktopEntry* de = folderItems[i];
        if (!de->hidden() && !de->noDisplay()) {
            AppMenuButton* newButton = new AppMenuButton(*de, buttonColumns[activeColumn()].size(), activeColumn());
            if (columnWidth == 0)columnWidth = newButton->getWidth();
            addButton(newButton);
        }
    }
    Rectangle<int> dest(getBounds());
    dest.setX(dest.getX() + columnWidth - 20);
    scrollTo(dest);
}

void AppMenu::closeFolder() {
    int columnWidth = 0;
    for (int i = buttonColumns[activeColumn()].size(); i >= 0; i--) {
        AppMenuButton * toDelete = buttonColumns[activeColumn()][i];
        if (columnWidth == 0)columnWidth = toDelete->getWidth();
        toDelete->removeFromDesktop();
        buttonColumns[activeColumn()].pop_back();
        delete toDelete;
    }
    buttonColumns.pop_back();
    selected.pop_back();

    Rectangle<int> dest(getBounds());
    int x = dest.getX() - columnWidth;
    if (x < 0)x = 0;
    dest.setX(x);
    scrollTo(dest);
}

void AppMenu::buttonClicked(Button* buttonClicked) {
    if (selected[activeColumn()] != NULL) {
        selected[activeColumn()]->setSelected(false);
        selected[activeColumn()]->repaint();
    }
    AppMenuButton * appClicked = (AppMenuButton *) buttonClicked;
    if (selected[activeColumn()] == appClicked) {
        if (appClicked->isFolder()) {
            openFolder(appClicked->getAppName());
        }
    } else {
        selected[activeColumn()] = appClicked;
        selected[activeColumn()]->setSelected(true);
        selected[activeColumn()]->repaint();

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
        scrollTo(dest);

    }
}

void AppMenu::selectIndex(int index) {
    if (index < buttonColumns[activeColumn()].size()
            && index >= 0)buttonColumns[activeColumn()][index]->triggerClick();
}

void AppMenu::selectNext() {
    if (selected[activeColumn()] == NULL)selectIndex(0);
    else selectIndex(selected[activeColumn()]->getIndex() + 1);
}

void AppMenu::selectPrevious() {
    if (selected[activeColumn()] == NULL)selectIndex(0);
    else selectIndex(selected[activeColumn()]->getIndex() - 1);
}

void AppMenu::clickSelected() {
    if (selected[activeColumn()] != NULL)selected[activeColumn()]->triggerClick();
}

int AppMenu::activeColumn() {
    return selected.size() - 1;
}

void AppMenu::scrollTo(Rectangle<int> dest) {
    Desktop::getInstance().getAnimator().animateComponent(this, dest, getAlpha(), 100, true, 1, 1);
}

void AppMenu::addButton(AppMenuButton* appButton) {
    int index = appButton->getIndex();
    int column = appButton->getColumn();
    int w = appButton->getWidth();
    int h = appButton->getHeight();
    int x = column*w;
    int y = h * this->buttonColumns[column].size();
    appButton->setBounds(x, y, w, h);
    addAndMakeVisible(appButton);
    appButton->setEnabled(true);
    appButton->setVisible(true);
    appButton->addListener(this);
    this->nameMap[appButton->getName()] = appButton;
    this->buttonColumns[column].push_back(appButton);
    if ((x + w) > getWidth())setBounds(getX(), getY(), x + w, getHeight());
    if ((index + 1) * h > getHeight())setBounds(getX(), getY(), getWidth(), index + 1 * h);
}