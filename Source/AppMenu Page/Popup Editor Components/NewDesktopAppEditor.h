/*
  ==============================================================================

    NewDesktopAppEditor.h
    Created: 23 Jan 2018 10:44:43pm
    Author:  anthony

  ==============================================================================
*/

#pragma once
#include "../DesktopEntry.h"
#include "AppMenuPopupEditor.h"

class NewDesktopAppEditor : public AppMenuPopupEditor {
public:
    
    NewDesktopAppEditor(IconThread& iconThread,
            std::function<void() > onConfirm = []() {
            });
};