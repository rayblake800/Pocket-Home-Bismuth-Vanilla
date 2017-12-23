/* 
 * File:   AppMenu.cpp
 * Author: anthony
 * Created on December 19, 2017, 1:37 PM
 */


#include <sstream>
#include <functional>
#include "Utils.h"
#include "AppMenu.h"

AppMenu::AppMenu(const var &configJson) {
    String jsonDebug=JSON::toString(configJson,false);
    DBG(jsonDebug);
    buttonWidth=configJson["menuButtonWidth"];
    buttonHeight=configJson["menuButtonHeight"];
    selected.push_back(NULL);
    buttonColumns.emplace(buttonColumns.begin());
    std::vector<DesktopEntry> categories = desktopEntries.getMainCategories(true);
    for (int i = 0; i < categories.size(); i++) {
        addButton(new AppMenuButton(categories[i],
                buttonColumns[activeColumn()].size(), activeColumn(),
                buttonWidth,buttonHeight));
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
    selected.push_back(NULL);
    buttonColumns.push_back(std::vector<AppMenuButton*>());
    DBG(String("found ") + String(folderItems.size()) + " items in " + categoryName);
    for (int i = 0; i < folderItems.size(); i++) {
        DesktopEntry desktopEntry = folderItems[i];
        if (!desktopEntry.hidden() && !desktopEntry.noDisplay()) {
            AppMenuButton* newButton = new AppMenuButton(desktopEntry,
                    buttonColumns[activeColumn()].size(), activeColumn(),
                    buttonWidth,buttonHeight);
            addButton(newButton);
        }
    }
    Rectangle<int> dest(getBounds());
    dest.setX(dest.getX() - buttonWidth + 30);
    scrollTo(dest);
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

    Rectangle<int> dest(getBounds());
    int x = dest.getX() + buttonWidth;
    if (x > 0)x = 0;
    dest.setX(x);
    scrollTo(dest);
}

//handle AppMenuButton clicks
void AppMenu::buttonClicked(Button* buttonClicked) {
    AppMenuButton * appClicked = (AppMenuButton *) buttonClicked;
    while (appClicked->getColumn() < activeColumn()) {
        closeFolder();
    }
    if (selected[activeColumn()] == appClicked) {
        if (appClicked->isFolder()) {
            openFolder(appClicked->getAppName());
        }
    } else {
        if (selected[activeColumn()] != NULL) {
            selected[activeColumn()]->setSelected(false);
            selected[activeColumn()]->repaint();
        }
        selected[activeColumn()] = appClicked;
        selected[activeColumn()]->setSelected(true);
        selected[activeColumn()]->repaint();

        //move AppMenu to center the selected button, if it's not near an edge
        int buttonPos = selected[activeColumn()]->getY();
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
    if ((y + h) > getHeight())setBounds(getX(), getY(), getWidth(), y + h);
}