#pragma once
#include "ComponentKey.h"

namespace ComponentConfigKeys
{
    //######################### Text Size Keys #################################
    static const ComponentKey smallTextKey
        ("small text", ConfigKey::doubleType);
    static const ComponentKey mediumTextKey
        ("medium text", ConfigKey::doubleType);
    static const ComponentKey largeTextKey
        ("large text", ConfigKey::doubleType;

    //######################### UI Component Data ##############################
    //Defines all component types managed in the config file
    static const juce::Identifier scrollingAppMenuKey
            ("scrolling app menu");
    static const juce::Identifier pagedAppMenuKey
            ("paged app menu");
    static const juce::Identifier menuFrameKey
            ("menu frame");
    static const juce::Identifier batteryIconKey
            ("battery");
    static const juce::Identifier batteryPercentKey
            ("battery percent text");
    static const juce::Identifier clockLabelKey
            ("time");
    static const juce::Identifier wifiIconKey
            ("wifi");
    static const juce::Identifier powerButtonKey
            ("power button");
    static const juce::Identifier settingsButtonKey
            ("settings button");
    static const juce::Identifier popupMenuKey
            ("popup menu");
    static const juce::Identifier pageLeftKey
            ("left arrow button");
    static const juce::Identifier pageRightKey
            ("right arrow button");
    static const juce::Identifier pageUpKey
            ("up arrow button");
    static const juce::Identifier pageDownKey
            ("down arrow button");
    static const juce::Identifier settingsListBtnKey
            ("settings list button");
    static const juce::Identifier spinnerKey
            ("loading spinner");
}
