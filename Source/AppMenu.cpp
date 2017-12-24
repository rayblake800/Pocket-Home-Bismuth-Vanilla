/* 
 * File:   AppMenu.cpp
 * Author: anthony
 * Created on December 19, 2017, 1:37 PM
 */


#include <sstream>
#include <functional>
#include <stdlib.h>
#include <vector>
#include "Utils.h"
#include "AppMenu.h"

AppMenu::AppMenu(const var &configJson,int xPos,int yPos) :
xPos(xPos),yPos(yPos){
    buttonWidth = configJson["menuButtonWidth"];
    buttonHeight = configJson["menuButtonHeight"];
    selected.push_back(NULL);
    columnTops.push_back(yPos);
    buttonColumns.emplace(buttonColumns.begin());

    //read in main page apps from config
    Array<var>* pagesData = configJson["pages"].getArray();
    if (pagesData) {
        for (const var &page : *pagesData) {
            String name = page["name"].toString();
            if (name == "Apps") {
                var items = page["items"];
                for (const var &item : *items.getArray()) {
                    if (item["name"].isString() 
                            && item["shell"].isString() 
                            && item["icon"].isString()) {
                        DBG(String("AppMenu:Found app in config:")+item["name"].toString());
                        addButton(new AppMenuButton(DesktopEntry(item),
                                buttonColumns[activeColumn()].size(), activeColumn(),
                                buttonWidth, buttonHeight));
                    }
                }
            }
        }
    }
    std::vector<DesktopEntry> categories = desktopEntries.getMainCategories(true);
    for (int i = 0; i < categories.size(); i++) {
        addButton(new AppMenuButton(categories[i],
                buttonColumns[activeColumn()].size(), activeColumn(),
                buttonWidth, buttonHeight));
    }
    DBG(String("added ") + String(buttonColumns[activeColumn()].size()) + " buttons");
}

AppMenu::~AppMenu() {
    while (!buttonColumns.empty())closeFolder();
}

/**
 * Open an application category folder, creating AppMenuButtons for all
 * associated desktop applications.
 */
void AppMenu::openFolder(String categoryName) {
    std::vector<DesktopEntry> folderItems = desktopEntries.getCategoryEntries(categoryName);
    if (folderItems.empty())return;
    int columnTop=yPos;
    if(selected[activeColumn()]!=NULL){
        columnTop=selected[activeColumn()]->getY();
    }
    selected.push_back(NULL);
    columnTops.push_back(columnTop);
    buttonColumns.push_back(std::vector<AppMenuButton*>());
    DBG(String("found ") + String(folderItems.size()) + " items in " + categoryName);
    for (int i = 0; i < folderItems.size(); i++) {
        DesktopEntry desktopEntry = folderItems[i];
        if (!desktopEntry.hidden() && !desktopEntry.noDisplay()) {
            AppMenuButton* newButton = new AppMenuButton(desktopEntry,
                    buttonColumns[activeColumn()].size(), activeColumn(),
                    buttonWidth, buttonHeight);
            addButton(newButton);
        }
    }
    scrollToSelected();
}

//close the topmost open folder, removing all contained buttons

void AppMenu::closeFolder() {
    for (int i = buttonColumns[activeColumn()].size() - 1; i >= 0; i--) {
        AppMenuButton * toDelete = buttonColumns[activeColumn()][i];
        toDelete->removeFromDesktop();
        buttonColumns[activeColumn()].pop_back();
        delete toDelete;
    }
    buttonColumns.pop_back();
    selected.pop_back();
    columnTops.pop_back();
    scrollToSelected();
}

//handle AppMenuButton clicks

void AppMenu::buttonClicked(Button * buttonClicked) {
    AppMenuButton * appClicked = (AppMenuButton *) buttonClicked;
    while (appClicked->getColumn() < activeColumn()) {
        closeFolder();
    }
    if (selected[activeColumn()] == appClicked) {
        if (appClicked->isFolder()) {
            openFolder(appClicked->getAppName());
        }
    } else {
        selectIndex(appClicked->getIndex());
    }
}

void AppMenu::selectIndex(int index) {
    int column = activeColumn();
    if (index >= buttonColumns[column].size()
            || index < 0
            || selected[column] == buttonColumns[column][index])return;
    if (selected[column] != NULL) {
        selected[column]->setSelected(false);
        selected[column]->repaint();
    }
    selected[column] = buttonColumns[column][index];
    selected[column]->setSelected(true);
    selected[column]->repaint();
    //move AppMenu to center the selected button, if it's not near an edge
    scrollToSelected();
}

//Select the next appMenuButton in the active button column.

void AppMenu::selectNext() {
    if (selected[activeColumn()] == NULL)selectIndex(0);
    else selectIndex(selected[activeColumn()]->getIndex() + 1);
}

//Select the previous appMenuButton in the active button column.

void AppMenu::selectPrevious() {
    if (selected[activeColumn()] == NULL)selectIndex(0);
    else selectIndex(selected[activeColumn()]->getIndex() - 1);
}

//Trigger a click for the selected button.

void AppMenu::clickSelected() {
    if (selected[activeColumn()] != NULL)selected[activeColumn()]->triggerClick();
}

//return the index of the active button column.

int AppMenu::activeColumn() {
    return selected.size() - 1;
}

void AppMenu::scrollToSelected() {
    int column = activeColumn();
    AppMenuButton* selectedButton = selected[column];
    Rectangle<int>dest = getBounds();
    if (selectedButton != NULL) {
        int buttonPos = selectedButton->getY();
        int screenHeight = Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight();
        int distanceFromCenter = abs(buttonPos-getY()+screenHeight/2);
        //only scroll vertically if selected button is outside the center 3/5 
        if(distanceFromCenter>screenHeight/5*3){
            dest.setY(columnTops[column]-buttonPos + screenHeight / 2 - buttonHeight / 2);
        }
        if (dest.getY() > columnTops[column]) {
            dest.setY(columnTops[column]);
        } else if (getHeight() > screenHeight && dest.getBottom() < screenHeight) {
            dest.setBottom(screenHeight);
        }
    } else dest.setY(columnTops[column]);
    if (column == 0)dest.setX(xPos);
    else {
        dest.setX(xPos-column * buttonWidth + buttonHeight);
    }
    ComponentAnimator& animator = Desktop::getInstance().getAnimator();
    if (animator.isAnimating(this)) {
        animator.cancelAnimation(this, false);
    }
    animator.animateComponent(this, dest, getAlpha(), 100, true, 1, 1);
}

void AppMenu::addButton(AppMenuButton * appButton) {
    int index = appButton->getIndex();
    int column = appButton->getColumn();
    int w = appButton->getWidth();
    int h = appButton->getHeight();
    int x = xPos+column*w;
    int y = columnTops[column]+h*buttonColumns[column].size();
    appButton->setBounds(x, y, w, h);
    addAndMakeVisible(appButton);
    appButton->setEnabled(true);
    appButton->setVisible(true);
    appButton->addListener(this);
    this->nameMap[appButton->getName()] = appButton;
    this->buttonColumns[column].push_back(appButton);
    if ((x + w) > getWidth())setBounds(getX(), getY(), x + w, getHeight());
    if ((y + h) > getHeight())setBounds(getX(), getY(), getWidth(), y + h);
}