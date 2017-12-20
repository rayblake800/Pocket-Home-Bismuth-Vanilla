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

class AppMenu : public Component,
public Button::Listener{
public:
    AppMenu();
    virtual ~AppMenu();
    void buttonClicked (Button* buttonClicked) override;
    bool keyPressed(const KeyPress &key) override;
private:
    ScopedPointer<AppMenuButton>* launchButtons=NULL;
    AppMenuButton* selected=NULL;
    int numButtons;
};

#endif /* APPMENU_H */

